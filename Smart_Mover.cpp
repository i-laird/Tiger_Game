#include "Smart_Mover.h"

void Smart_Mover::determine_rows() {
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
}


Move_t Smart_Mover::off_move_handling() {
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
        // pick column furthest from tiger
        off_col = 0;
        if (current.get_tiger().location.col < (NUM_COL - 1) / 2) {
            off_col = NUM_COL - 1;
        }
        // if not to tiger cage, finish creating off move
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

    Move_t** moves_to_do = find_moves_to_do();
    if(moves_to_do == nullptr) {
        return;
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
        }
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

    if(!move_ready) {
        // determine rows
        determine_rows();
        if(front_row <= 4) {
            to_tiger_cage = true;
        }
        // look for path
        else {
            this->path.clear();
            this->desired.clear();
            this->search_for_state();
            if(move_ready) {
                move_from_search = true;
            }
        }
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


    // if to tiger cage, try a tiger cage move
    if(to_tiger_cage) {
        ///to_do = kill_tiger_handling();
        to_do = finish_off_tiger();
        if (to_do != NULL_MOVE) {
            move_ready = true;
        }
    }

    // if no move found still, try an off move
    if(!move_ready && !to_tiger_cage){
        off_move_active = !off_move_active;
    }
    // if to tiger cage, turn off the off move
    if(to_tiger_cage) {
        off_move_active = false;
    }

    // handle off move
    Move_t off_move_to_do = off_move_handling();
    if(off_move_ready) {
        to_do = off_move_to_do;
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

    return to_do;
}


set<Point_t> Smart_Mover::tiger_reachable_pos() {
    // determine positions reachable by tiger using a bfs
    set<Point_t> tiger_reachable;
    queue<Point_t> frontier;
    frontier.push(current.get_tiger().location);
    tiger_reachable.insert(current.get_tiger().location);
    Point_t orig_tiger_pos = current.get_tiger().location;
    while(!frontier.empty()) {
        Point_t tig_pos = frontier.front();
        frontier.pop();
        current.set_tiger(make_tiger(tig_pos));
        auto t_moves = game.validMoves(current, current.get_tiger());
        // for each position reachable from tig_pos, mark as reachable
        // and if not been there before, add to the frontier
        for(int i = 0; i < t_moves.second.second; ++i) {
            Point_t dest = t_moves.first[i];
            if(tiger_reachable.insert(dest).second) {
                frontier.push(dest);
            }
        }
        // free memory
        if(t_moves.first) {
            delete [] t_moves.first;
            t_moves.first = nullptr;
        }
        if(t_moves.second.first) {
            delete [] t_moves.second.first;
            t_moves.second.first = nullptr;
        }
    }
    // return tiger to original position
    current.set_tiger(make_tiger(orig_tiger_pos));
    // remove current tiger position from reachable positions
    tiger_reachable.erase(current.get_tiger().location);

    return tiger_reachable;
}


bool Smart_Mover::leads_to_cycle(Move_t mv) {
    bool cycling = false;
    // see if cycling
    Point_t net_mvmt = ZERO_VECT;
    Point_t mvmt_todo = mv.destination - mv.token.location;
    for(auto prev = prev_moves.rbegin(); prev != prev_moves.rend() && !cycling; ++prev) {
        net_mvmt += prev->destination;
        net_mvmt -= prev->token.location;
        if(net_mvmt - mvmt_todo == ZERO_VECT) {
            cycling = true;
        }
    }
    return cycling;
}



Move_t Smart_Mover::finish_off_tiger() {
    Move_t to_do = NULL_MOVE;

    // if have cached moves, do it
    if(!q.empty()) {
        to_do = q.front();
        q.pop();

        // record move
        if(prev_moves.size() > MAX_CYCLING_CHECK) {
            prev_moves.pop_front();
        }
        prev_moves.push_back(to_do);

        return to_do;
    }

    set<Point_t> tiger_reachable = tiger_reachable_pos();
    unsigned long num_reachable = tiger_reachable.size();
    // look for moves into tiger reachable positions
    // and for moves that fill in behind those ones
    vector<Move_t> moves_into_t_reachable;
    vector<set<Move_t> >fill_in_behind_mvs; // map from a move into
                                            // a tiger reachable pos
                                            // to moves that fill in
                                            // behind it
    // for each man
    State cur = current;
    for(int i = 1; i < cur.size(); ++i) {
        Token_t man = cur[i];
        // get man moves
        auto moves = game.validMoves(current, man);
        // see if moves into t_reachable
        for(int j = 0; j < moves.second.second; ++j) {
            Point_t dest = moves.first[j];
            if(tiger_reachable.find(dest) != tiger_reachable.end()) {
                Move_t mv = make_move(man, dest);
                moves_into_t_reachable.emplace_back(mv);
                fill_in_behind_mvs.emplace_back(set<Move_t>());
                // look for any moves that fill in behind it
                current.do_move(mv);
                for(int k = 1; k < cur.size(); ++k) {
                    Token_t next_man = cur[k];
                    // don't look at man moving right now
                    if(next_man.location != mv.destination) {
                        // see if moves into man positions
                        Move_t fill_in = make_move(next_man, mv.token.location);
                        // if fills in behind the mv, add to set of Moves that
                        // fill in behind it
                        if(game.isValidMove(current, fill_in)) {
                            fill_in_behind_mvs.back().insert(fill_in);
                        }
                    }
                }
                current.do_move(-mv);
            }
        }

        // free memory
        if(moves.first) {
            delete[] moves.first;
            moves.first = nullptr;
        }
        if(moves.second.first) {
            delete [] moves.second.first;
            moves.second.first = nullptr;
        }

    }

    // try each pair of move / fill_in behind it to see if any reduces
    // tiger reachable positions
    bool move_found = false;
    for(int i = 0; i < moves_into_t_reachable.size() && !move_found; ++i) {
        Move_t mv = moves_into_t_reachable[i];
        current.do_move(mv);
        unsigned long new_reachable = tiger_reachable_pos().size();
        if(new_reachable < num_reachable || (new_reachable == num_reachable &&
                                            mv.destination.row < mv.token.location.row)) {
            bool cycling = leads_to_cycle(mv);
            bool sec = secure(&current, &game);
            if(sec && !cycling) {
                move_found = true;
                q.push(mv);
            }
        }
        auto s = fill_in_behind_mvs[i].begin();
        while(s != fill_in_behind_mvs[i].end() && !move_found) {
            Move_t mv2 = *s;
            // if tiger can prevent this move, not a good move
            Move_t prevent_mv2 = make_move(current.get_tiger(), mv2.destination);
            bool preventable = game.isValidMove(current, prevent_mv2);
            if(!preventable && !tiger_can_jump(&current, &game)) {
                current.do_move(mv2);
                // if tiger cannot jump after and does not increase reachable positions,
                // do it
                new_reachable = tiger_reachable_pos().size();
                if (!tiger_can_jump(&current, &game) &&
                        (new_reachable < num_reachable || (new_reachable == num_reachable
                                                          && mv.destination.row < mv.token.location.row
                                                          && mv2.destination.row < mv2.token.location.row))) {
                    bool sec = secure(&current, &game);
                    bool cycling = leads_to_cycle(mv2);

                    if(sec && !cycling) {
                        move_found = true;
                        q.push(mv);
                        q.push(mv2);
                    }
                }
                current.do_move(-mv2);
            }
            ++s;
        }
        current.do_move(-mv);
    }

    // if no move found, then find the man furthest from tiger
    // in one norm with a move that decreases their distance and do it
    Move_t towards_tiger = NULL_MOVE;
    if(!move_found) {
        Token_t tiger = current.get_tiger();
        for(int i = 1; i < cur.size() && !move_found; ++i) {
        Token_t man = cur[i];
            auto moves = game.validMoves(current, man);
            for (int j = 0; j < moves.second.second; ++j) {
                Point_t dest = moves.first[j];
                Move_t mv = make_move(man, dest);
                bool cycling = leads_to_cycle(mv);
                // if move moves the man closer to the tiger
                if(one_norm(tiger.location - dest) <
                        one_norm(tiger.location - man.location) && !cycling) {
                    towards_tiger = mv;
                    move_found = true;
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
        if(move_found) {
            q.push(towards_tiger);
        }
    }

    if(!q.empty()) {
        to_do = q.front();
        q.pop();

        // record move
        if(prev_moves.size() > MAX_CYCLING_CHECK) {
            prev_moves.pop_front();
        }
        prev_moves.push_back(to_do);
    }


    return to_do;
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



