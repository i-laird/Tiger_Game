#include "Smart_Mover.h"

void Smart_Mover::determine_rows() {
    const Unordered_State copy = current;
    // pretend off move not done
    /*if(off_move_active) {
        this->current.do_move(-off_move);
    }*/

    set<int> row_to_col[NUM_ROW];

    /// back_row is the furthest back token
    this->back_row = -1;
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current.rows_in_col(c).begin();
        while(r != current.rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            back_row = max(back_row, *r);
            ++r;
        }
    }

    /// if all tokens on back_row or back_row - 1, front_row
    /// is back_row - 2 (the next row to move to)
    if(row_to_col[back_row].size() == NUM_COL &&
       row_to_col[back_row - 1].size() == NUM_COL) {
        this->front_row = back_row - 2;
    }
    /// else if all columns on back_row - 1 and back_row - 2 except
    /// for one column which is on back_row - 1 and back_row, and tiger
    /// is blocking that column from moving up
    /// front_row is back_row - 3
    else if (row_to_col[back_row - 1].size() == NUM_COL &&
             row_to_col[back_row - 2].size() == NUM_COL - 1 &&
             row_to_col[back_row].size() == 1) {
        int col = *row_to_col[back_row].begin();
        if(row_to_col[back_row - 2].find(col) == row_to_col[back_row - 2].end()){
            if(this->current.get_tiger().location.row >= back_row - 3 &&
               this->current.get_tiger().location.col == col) {
                // if lagging column is
                // - on middle if in lower half
                // - not on an edge if on upper half
                if((front_row >= 8 && col == (NUM_COL - 1) / 2) ||
                   (front_row < 8 && (col == 0 || col == NUM_COL - 1))) {
                    this->front_row = back_row - 3;
                }
            }
        }
    }

    /// otherwise front_row is left unchanged
    // re-do off move
    /*if(off_move_active) {
        this->current.do_move(off_move);
    }*/

    if(copy != current) {
        copy == current;
    }
}


Move_t Smart_Mover::off_move_handling() {
    Unordered_State copy = current;
    // ensure that if an off move is purported to exist, it actually does
    // and it can be undone
    if(off_move != NULL_MOVE && (current.get_tiger().location == off_move.destination
                                || current.is_occupied(off_move.token.location) ||
                                !current.is_occupied(off_move.destination))) {
        off_move = NULL_MOVE;
        off_move_active = false;
    }

    off_move_ready = false;
    /// if token in danger because of off move, undo off move
    // or if an off move too far bback, undo it
    Move_t off_move_capture = make_move(current.get_tiger(),
                                        off_move.token.location);
    if((off_move_active && off_move != NULL_MOVE &&
       game.isValidMove(current, off_move_capture))) {
        off_move_active = false;
    }

    Move_t to_do = NULL_MOVE;
    /// if off_move_active but off_move is NULL_MOVE, set an off move
    if(off_move_active && off_move == NULL_MOVE) {
        int off_col;
        if(!to_tiger_cage) {
            // pick column furthest from tiger
            off_col = 0;
            if (current.get_tiger().location.col < (NUM_COL - 1) / 2) {
                off_col = NUM_COL - 1;
            }
        }
        else {
            // pick column furthest from tiger with a man not in the cage
            // and not in stage positions
            off_col = -1;
            int tiger_col = current.get_tiger().location.col;
            for(int c = 0; c < NUM_COL - 1; ++c) {
                // if already found a potential off move and this column is no
                // further from tiger, then look for another
                if(off_col >= 0 && abs(off_col - tiger_col) >= abs(c - tiger_col)) {
                    continue;
                }
                bool off_move_found = false;
                auto r = current.rows_in_col(c).begin();
                while (r != current.rows_in_col(c).end() && !off_move_found) {
                    Point_t pt = make_point(*r,c);
                    ++r;
                    // don't move a man in stage position or in tiger cage
                    bool bad_choice = false;
                    if(pt.row < CAGE_ENTRANCE.row) {
                        bad_choice = true;
                    }
                    for(int k = 0; k < STAGE_POS_SIZE; ++k) {
                        if(pt == STAGE_POSITIONS[k]) {
                            bad_choice = true;
                        }
                    }
                    if(bad_choice) {
                        continue;
                    }
                    // if not a bad choice, make the man
                    Token_t man = make_man(pt);
                    auto moves = game.validMoves(current, man);
                    // if man has moves, pick one going to furthest column from tiger
                    if(moves.second.second > 0) {
                        off_move_found = true;
                        off_move.token = man;
                        off_move.destination = moves.first[0];
                        for(int k = 1; k < moves.second.second; ++k) {
                            if(abs(off_move.destination.col - tiger_col) <
                                    abs(moves.first[k].col - tiger_col)) {
                                off_move.destination = moves.first[k];
                            }
                        }
                    }
                    if(moves.first) {
                        delete [] moves.first;
                        moves.first = nullptr;
                    }
                    if(moves.second.first) {
                        delete [] moves.second.first;
                        moves.second.first = nullptr;
                    }
                }
            }
        }
        // if not to tiger cage, finish creating off move
        if(!to_tiger_cage) {
            int row = -1;
            // pick lowest token in off_col
            auto r = current.rows_in_col(off_col).begin();
            while (r != current.rows_in_col(off_col).end()) {
                row = max(*r, row);
                ++r;
            }

            // create off move and set to_do to it
            Token_t man = make_man(make_point(row, off_col));
            off_move = make_move_in_direction(man, DOWN);
        }
        // store the off move to-do and set off_move_ready to true
        to_do = off_move;
        off_move_ready = true;
    }
    /// else if not off_move_active and off_move is not NULL_MOVE, undo
    /// the off_move
    else if(!off_move_active && off_move != NULL_MOVE) {
        to_do = -off_move;
        off_move = NULL_MOVE;
        off_move_ready = true;
    }

    if(copy != current) {
        copy == current;
    }

    /// if doing an off move, cancel all other plans
    if(off_move_ready) {
        move_ready = false;
        path.clear();
        desired.clear();
        this->special_moves.clear_moves();
    }

    return to_do;
}


Move_t** Smart_Mover::find_moves_to_do() {
    /// store forward moves for each column except for the
    /// off_move column, and if there are more than two
    /// men in any column return nullptr
    Move_t** moves_to_do = new Move_t*[NUM_COL];
    for(int c = 0; c < NUM_COL; ++c) {
        moves_to_do[c] = new Move_t[2];
        moves_to_do[c][0] = moves_to_do[c][1] = NULL_MOVE;
        if(c != off_move.destination.col) {
            auto r = current.rows_in_col(c).begin();
            while(r != current.rows_in_col(c).end()) {
                Token_t man = make_man(make_point(*r, c));
                if(moves_to_do[c][0] != NULL_MOVE &&
                   moves_to_do[c][1] != NULL_MOVE) {
                    for(int i = 0; i < NUM_COL; ++i) {
                        if(moves_to_do[i]) {
                            delete[] moves_to_do[i];
                            moves_to_do[i] = nullptr;
                        }
                    }
                    if(moves_to_do) {
                        delete[] moves_to_do;
                        moves_to_do = nullptr;
                    }
                    return nullptr;
                }
                else if(moves_to_do[c][0] == NULL_MOVE) {
                    moves_to_do[c][0] = make_move_in_direction(man, UP);
                }
                else {
                    moves_to_do[c][1] = make_move_in_direction(man, UP);
                    // ensure the first move to do in this column is the token
                    // in front
                    if(moves_to_do[c][0].destination >
                                              moves_to_do[c][1].destination){
                        Move_t temp = moves_to_do[c][0];
                        moves_to_do[c][0] = moves_to_do[c][1];
                        moves_to_do[c][1] = temp;
                    }
                    // if the guy in front already has a space between
                    // him and the guy below, don't try to move the top guy again
                    if(moves_to_do[c][1].destination.row - moves_to_do[c][0].destination.row > 1) {
                        moves_to_do[c][0] = moves_to_do[c][1];
                        moves_to_do[c][1] = NULL_MOVE;
                    }

                }
                ++r;
            }
        }
    }

    /// ensure that not moving past front line
    for(int c = 0; c < NUM_COL; ++c) {
        if(moves_to_do[c][0].destination.row < front_row) {
            moves_to_do[c][0] = NULL_MOVE;
            if(moves_to_do[c][1].destination.row < front_row + 1) {
                moves_to_do[c][1] = NULL_MOVE;
            }
        }
    }

    return moves_to_do;
}


Move_t Smart_Mover::try_for_specific_move() {
    Move_t to_do = NULL_MOVE;
    special_moves.set_front_back_row(front_row, back_row);

    // if no move ready, then try a special move
    if(special_moves.handle_special_case(off_move)) {
        to_do = special_moves.get_move();
        if(to_do != NULL_MOVE) {
            path.clear();
            desired.clear();
            move_ready = true;
        }
    }

    return to_do;
}


void Smart_Mover::search_for_state() {

    Unordered_State copy = current;
    if(copy != current) {
        copy == current;
    }

    Move_t** moves_to_do = find_moves_to_do();
    if(moves_to_do == nullptr) {
        return;
    }
    if(copy != current) {
        copy == current;
    }

    move_ready = false;
    int off_col = off_move.destination.col;
    /// look for moves which move forward tokens up to two adjacent columns
    /// from a set of 3
    for(int c = 0; c < NUM_COL - 2 && !move_ready; ++c) {
        if(c == off_col || c + 1 == off_col || c + 2 == off_col) {
            continue;
        }
        desired.clear();
        path.clear();
        // desired states are those which move up either 1 or
        // 2 of the 3 columns
        int ndx = 0;
        int req_moves[6] = {0,0,0,0,0,0};
        for(int i = 0; i < 3; ++i) {
            for(int j = i; j < 3; ++j) {
                Move_t mv;
                Hash_val desired_hash = hash_locs(current, back_row);
                for(int col = 0; col < 3; ++col) {
                    for(int k = 0; k < 2; ++k) {
                        if(col != i && col != j) {
                            mv = moves_to_do[c + col][k];
                            if(mv != NULL_MOVE) {
                                ++req_moves[ndx];
                                current.do_move(mv);
                                desired_hash = next_hash(mv, desired_hash, back_row);
                            }
                        }
                    }
                }
                // if the suggested desired state is secure, add it to the set
                // of desired states
                if(secure(&current, &game, off_move) && req_moves[ndx] > 0) {
                    desired.insert(desired_hash);
                }
                else {
                    req_moves[ndx] = 0;
                }
                ++ndx;

                // undo moves
                for(int col = 2; col >= 0; --col) {
                    for(int k = 1; k >= 0; --k) {
                        if(col!= i && col != j) {
                            mv = moves_to_do[c + col][k];
                            if(mv != NULL_MOVE) {
                                current.do_move(-mv);
                            }
                        }
                    }
                }
                if(copy != current) {
                    copy == current;
                }
            }
        }

        // create transition, and have it look for a path to the desired states
        if(!desired.empty()) {
            Transition t(&current, &desired, &game, back_row, make_pair(c, c+3),
                         make_pair(front_row, back_row + 1));
            int min_req = INFTY;
            int max_req = -1;
            for (int req_move : req_moves) {
                max_req = max(max_req, req_move);
                if(req_move > 0) {
                    min_req = min(req_move, min_req);
                }
            }
            move_ready = t.find_path_to_state(min(4,max_req), min_req);
            if(move_ready) {
                path = t.get_path();
            }
            if(copy != current) {
                copy == current;
            }
        }
    }

    if(copy != this->current) {
        copy == this->current;
    }

    // free memory
    for(int c = 0; c < NUM_COL; ++c) {
        if(moves_to_do[c]) {
            delete[] moves_to_do[c];
            moves_to_do[c] = nullptr;
        }
    }
    if(moves_to_do) {
        delete[] moves_to_do;
        moves_to_do = nullptr;
    }
}


Move_t Smart_Mover::fail_safe(Move_t suggested) {
    // first call safety failsafe
    Move_t to_do = safety_fail_safe(suggested);
    if(to_do != NULL_MOVE) {
        suggested = to_do;
    }

    // now ensure validity
    if(!game.isValidMove(current, suggested)) {
        State cur = current;
        p_board(cur);
        cout << suggested << "\n";
        game.isValidMove(current, suggested);
        // if move not valid, look for a valid safe move
        bool fail_safe_found = false;
        suggested = NULL_MOVE;
        to_do = safety_fail_safe(suggested);
        if(to_do != NULL_MOVE) {
            fail_safe_found = true;
        }
        // try each direction of moves and see if finds one that is valid
        // with no immediage jump
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        // if cannot find a safe valid move, just try to find a valid move
        for (int i = 0; i < 8 && !fail_safe_found; ++i) {
            for (int j = 1; j < cur.size() && !fail_safe_found; ++j) {
                Token_t man = cur[j];
                // don't try to move away from the middle
                if ((man.location.col < (NUM_COL - 1) / 2 && dirs[i].col < 0) ||
                    (man.location.col > (NUM_COL - 1) / 2 && dirs[i].col > 0)) {
                    continue;
                }
                Move_t mv = make_move_in_direction(man, dirs[i]);
                if (game.isValidMove(current, mv) &&
                    (mv.destination.row >= front_row || to_tiger_cage)) {
                    fail_safe_found = true;
                    to_do = mv;
                }
            }
        }
    }

    return to_do;
}


Move_t Smart_Mover::safety_fail_safe(Move_t suggested) {
    Move_t to_do = NULL_MOVE;

    bool did_move = false;
    if(game.isValidMove(current, suggested)) {
        current.do_move(suggested);
        did_move = true;
    }
    bool can_jump = tiger_can_jump(&current, &game);
    if(did_move) {
        current.do_move(-suggested);
    }

    if(can_jump || !did_move) {
        State cur = current;
        bool fail_safe_found = false;
        // try each direction of moves and see if can prevent the
        // capture
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        for (int i = 0; i < 8 && !fail_safe_found; ++i) {
            for (int j = 1; j < cur.size() && !fail_safe_found; ++j) {
                Token_t man = cur[j];
                // don't try to move away from the middle if tiger in cage
                bool bad_choice = false;
                if (current.get_tiger().location.row < CAGE_ENTRANCE.row &&
                        ((man.location.col < (NUM_COL - 1) / 2 && dirs[i].col < 0) ||
                        (man.location.col > (NUM_COL - 1) / 2 && dirs[i].col > 0))) {
                    bad_choice = true;
                }
                // try not to move off man
                if(man.location == off_move.destination) {
                    bad_choice = true;
                }
                // if at tiger cage, don't move any man in a STAGE_POSITION
                if(to_tiger_cage) {
                    for (auto stage_pos : STAGE_POSITIONS) {
                        if(man.location == stage_pos) {
                            bad_choice = true;
                        }
                    }
                }
                if(bad_choice) {
                    continue;
                }

                Move_t mv = make_move_in_direction(man, dirs[i]);
                if (game.isValidMove(current, mv) &&
                        (to_tiger_cage || mv.destination.row >= front_row)) {
                    current.do_move(mv);
                    if (!tiger_can_jump(&current, &game)) {
                        fail_safe_found = true;
                        to_do = mv;
                    }
                    current.do_move(-mv);
                }
            }
        }
    }


    return to_do;
}


Move_t Smart_Mover::execute_move() {
    Move_t to_do = NULL_MOVE; // men move to do
    const Unordered_State copy = this->current;

    Hash_val cur_hash = hash_locs(current, back_row);
    pair<Hash_val, Token_t> ndx = make_pair(cur_hash, current.get_tiger());
    // try a search move
    bool move_from_search = false;
    // see if move still ready from previous search
    if(move_ready) {
        auto next_move = path.find(ndx);
        if(next_move == path.end()) {
            move_ready = false;
        }
        else {
            move_from_search = true;
        }
    }

    // see if move still ready from previous specific move
    if(!move_from_search) {
        to_do = special_moves.get_move();
        if(to_do != NULL_MOVE) {
            move_ready = true;
            path.clear();
            desired.clear();
        }
    }

    if(copy != this->current) {
        cout << "shift";
    }

    if(!move_ready) {
        // determine rows
        determine_rows();
        if(copy != this->current) {
            cout << "shift";
        }
        if(front_row <= 4) {
            to_tiger_cage = true;
        }
        // look for path
        else {
            this->path.clear();
            this->desired.clear();
            this->search_for_state();
            if(copy != this->current) {
                copy == this->current;
            }
            if(move_ready) {
                move_from_search = true;
            }
        }
    }

    if(copy != this->current) {
        copy == this->current;
        cout << "shift";
    }

    // if move is ready from search, do it
    if(move_ready && !to_tiger_cage && move_from_search) {
        cur_hash = hash_locs(current, back_row);
        ndx = make_pair(cur_hash, current.get_tiger());
        to_do = path[ndx];
    }
    // if no move ready from the greedy move, try a special move
    else if(!move_ready){
        to_do = try_for_specific_move();
        if(to_do != NULL_MOVE) {
            move_ready = true;
        }
    }

    if(copy != this->current) {
        cout << "shift";
    }

    // if to tiger cage, try a tiger cage move
    if(to_tiger_cage) {
        to_do = kill_tiger_handling();
        if (to_do != NULL_MOVE) {
            move_ready = true;
        }
    }

    if(copy != this->current) {
        cout << "shift";
    }

    // if no move found still, try an off move
    if(!move_ready){// && !to_tiger_cage) {
        off_move_active = !off_move_active;
    }
    /*
    // undo off move if to tiger cage
    if(to_tiger_cage && off_move_active) {
        off_move_active = false;
    }
    */
    if(copy != this->current) {
        copy == current;
    }

    // handle off move
    Move_t off_move_to_do = off_move_handling();
    if(off_move_ready) {
        to_do = off_move_to_do;
    }

    if(copy != this->current) {
        cout << "shift";
    }

    // perform failsafe
    Move_t fs = fail_safe(to_do);
    if(fs != NULL_MOVE) {
        // if was performing an off move, undo that
        if(off_move_ready) {
            if (off_move_active) {
                off_move_active = false;
                off_move = NULL_MOVE;
            } else {
                off_move_active = true;
                off_move = -to_do;
            }
        }
        // if moving the off move token, turn off move active to false
        if(fs.token.location == off_move.destination) {
            off_move = NULL_MOVE;
            off_move_active = false;
        }
        move_ready = false;
        this->path.clear();
        this->desired.clear();
        this->special_moves.clear_moves();
        to_do = fs;
    }

    if(copy != this->current) {
        cout << "shift";
    }

    if(!game.isValidMove(current,to_do)) {
        p_board(current);
        cout << to_do;
    }

    // do the move in this object
    current.do_move(to_do);

    return to_do;
}


Move_t Smart_Mover::kill_tiger_handling() {

	Move_t ret = NULL_MOVE;
	Point_t tiger_loc = current.get_tiger().location;

	if (!q.empty()) {
		ret = q.front();
		q.pop();
		return ret;
	}

	State cur = current;
    // see if any move kills the tiger
    for(int i = 1; i < cur.size() && ret == NULL_MOVE; ++i) {
        Token_t man = cur[i];
        auto moves = game.validMoves(current, man);
        for(int j = 0; j < moves.second.second && ret == NULL_MOVE; ++j) {
            Move_t mv = make_move(man, moves.first[j]);
            current.do_move(mv);
            auto t_moves = game.validMoves(current, current.get_tiger());
            if(t_moves.second.second <= 0) {
                ret = mv;
            }
            // free memory
            if(t_moves.first) {
                delete[] t_moves.first;
                t_moves.first = nullptr;
            }
            if(t_moves.second.first) {
                delete[] t_moves.second.first;
                t_moves.second.first = nullptr;
            }
            // undo move
            current.do_move(-mv);
        }
        // free memory
        if(moves.first) {
            delete[] moves.first;
            moves.first = nullptr;
        }
        if(moves.second.first) {
            delete[] moves.second.first;
            moves.second.first = nullptr;
        }
    }
	// if found a kill-tiger move, do it
	if(ret != NULL_MOVE) {
	    return ret;
	}

	// ensure tiger is at top row / trapped in
    bool trapped_in = false;
	if(current.is_occupied(CAGE_ENTRANCE)){
	    if(current.get_tiger().location.row < 4) {
            trapped_in = true;
        }
	}
	if(!trapped_in) {
	    // for each man
		for (int i = 1; i < cur.size(); ++i) {
            Token_t man = cur[i];
            pair<Point_t*, pair<bool*, int>> val_moves = game.validMoves(current, man);
            // check each move the man can make that is not downward
            for (int j = 0; j < val_moves.second.second && q.empty(); j++) {
                Move_t mv = make_move(man, val_moves.first[j]);
                Point_t to = mv.destination, from = mv.token.location;
                // if moving downward, don't do this move
                if((to - from).row > 0) {
                    continue;
                }
                // if moving side-to-side away from the middle, don't do this move
                int middle = (NUM_COL - 1) / 2;
                if((to - from).row == 0 && abs(to.col - middle) > abs(from.col - middle)) {
                    continue;
                }
                // mv2 is in direction of mv but its destination is the source point of mv
                Move_t mv2 = make_move(make_man(from - (to - from)), from);

                // if mv, mv2 leaves the board secure and both are valid, do them
                current.do_move(mv);
                if(game.isValidMove(current, mv2)) {
                    // if tiger is too close, mv2 may not be guaranteed,
                    // so don't do it
                    if(one_norm(mv2.destination - tiger_loc) > 4) {
                        current.do_move(mv2);
                        if (secure(&current, &game)) {
                            q.push(mv);
                            q.push(mv2);
                        }
                        current.do_move(-mv2);
                    }
                }
                // else if mv leaves the board secure, do it
                if (q.empty() && secure(&current, &game)) {
                    q.push(mv);
                }
                current.do_move(-mv);
            }
            //free memory
            if (val_moves.first) {
                delete[] val_moves.first;
                val_moves.first = nullptr;
            }
            if (val_moves.second.first) {
                delete[] val_moves.second.first;
                val_moves.second.first = nullptr;
            }

            // if there is a move to do, return it
            if (!q.empty()) {
                ret = q.front();
                q.pop();
                return ret;
            }
		}
	}

	// if cage not staged, try to stage it
	if (!cage_staged()) {
		ret = stage_men();
	}

	// if no move yet found, try to move inside the cage
	if(ret == NULL_MOVE && cage_staged()) {
	    ret = get_move_in_cage();
	}

	// if no move yet found, try to move into the cage
	if (ret == NULL_MOVE && cage_staged() && current.get_tiger().location.row < CAGE_ENTRANCE.row - 1) {
		ret = get_move_into_cage();
	}

	return ret;
}


bool Smart_Mover::cage_staged() {
    // the cage is staged iff every position in STAGE_POSITIONS is filled by a man
    bool staged = true;
	for (int i = 0; i < STAGE_POS_SIZE && staged; i++) {
		if (!current.is_occupied(STAGE_POSITIONS[i]) || current.get_tiger().location == STAGE_POSITIONS[i]) {
			staged = false;
		}
	}
	return staged;
}


Move_t Smart_Mover::stage_men() {
	Move_t move;
	// for each position that needs to be filled in order
    // for the cage to be staged
	for(int i = 0; i < STAGE_POS_SIZE; i++) {
	    // look for a move from a non-stage position towards an
        // unfilled STAGE_POSITION
		move = bfs_move_getter(&current, &game, STAGE_POSITIONS[i]);
		if (move != NULL_MOVE) {
			return move;
		}
	}

	return NULL_MOVE;
}


Move_t Smart_Mover::get_move_in_cage() {
    State cur = current;
	for (int i = 1; i < cur.size(); i++) {
        Token_t man = cur[i];
        if (man.location.row < 4) {
            auto moves = game.validMoves(current, man);
            for (int j = 0; j < moves.second.second && q.empty(); j++) {
                if (moves.first[j].row < man.location.row) {
                    Move_t mv = make_move(man, moves.first[j]);
                    Point_t to = mv.destination, from = mv.token.location;
                    Move_t mv2 = make_move(make_man(to), to + to - from);
                    current.do_move(mv);
                    bool moved = false;
                    if(game.isValidMove(current, mv2)) {
                        moved = true;
                        current.do_move(mv2);
                    }
                    if (secure(&current, &game)) {
                        q.push(mv);
                        if (moved) {
                            q.push(mv2);
                            current.do_move(-mv2);
                        }
                    }
                    current.do_move(-mv);
                }
            }
            if (moves.first) {
                delete[] moves.first;
                moves.first = nullptr;
            }
            if (moves.second.first) {
                delete[] moves.second.first;
                moves.second.first = nullptr;
            }
            if (!q.empty()) {
                Move_t ret = q.front();
                q.pop();
                return ret;
            }
        }
	}

	return NULL_MOVE;
}


Move_t Smart_Mover::get_move_into_cage() {
    // if there is no man in the cage entrance, or tiger not in cage / near entrance
    // of cage, there is no move into the cage
	if (!(current.is_occupied(CAGE_ENTRANCE) && current.get_tiger().location.row < 3)) {
		return NULL_MOVE;
	}

	Token_t man = make_man(make_point(4, 4));
	auto moves = game.validMoves(current, man);
	bool no_fill;
	for (int i = 0; i < moves.second.second && q.empty(); i++) {
	    no_fill = false;
		Move_t mv = make_move(man, moves.first[i]);
		Point_t to = mv.destination, from = mv.token.location;
		// ensure mv is into the tiger cage
        if((to - from).row >= 0) {
            continue;
        }
		Move_t mv2 = NULL_MOVE;
		Point_t fills[3]; // points from which it is valid to fill in behind
                          // the man entering the cage
		fills[0] = make_point(4, 3);
		fills[1] = make_point(5, 4);
		fills[2] = make_point(4, 5);

		// find a man in one of the fill positions and make the filling
        // move mv2
		for (auto fill : fills) {
			if (current.is_occupied(fill)) {
				mv2 = make_move(make_man(fill), from);
				no_fill = true;
				break;
			}
		}

		current.do_move(mv);
		current.do_move(mv2);
		if (secure(&current, &game)) {
			q.push(mv);
			q.push(mv2);
		}
		current.do_move(-mv2);
		current.do_move(-mv);
	}
	// free memory
	if (moves.first) {
		delete[] moves.first;
		moves.first = nullptr;
	}
	if (moves.second.first) {
		delete[] moves.second.first;
		moves.second.first = nullptr;
	}
    if (!no_fill && !q.empty()) {
        Move_t ret = q.front();
        q.pop();
        return ret;
    }

	return NULL_MOVE;
}


Smart_Mover::Smart_Mover(const State& s):Men_Mover(s){
    back_row = NUM_ROW - 1;
    front_row = back_row - 2;
    special_moves = Specific_Move_Handler(&current, &game);

    // about next move
    move_ready = false;
    off_move_ready = false;
    path.clear();
    desired.clear();

    // about off move
    off_move_active = false;
    off_move = NULL_MOVE;

    // whether or not to tiger
    to_tiger_cage = false;
}



