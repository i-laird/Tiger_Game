#include "Smart_Mover.h"

void Smart_Mover::determine_rows() {
    // pretend off move not done
    if(off_move_active) {
        this->current.do_move(-off_move);
    }

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
    if(off_move_active) {
        this->current.do_move(off_move);
    }
}


Move_t Smart_Mover::off_move_handling() {
    off_move_ready = false;
    /// if token in danger because of off move, undo off move
    Move_t off_move_capture = make_move(current.get_tiger(),
                                        off_move.token.location);
    if(off_move_active && off_move != NULL_MOVE &&
       game.isValidMove(current, off_move_capture)) {
        off_move_active = false;
    }

    Move_t to_do = NULL_MOVE;
    /// if off_move_active but off_move is NULL_MOVE, set an off move
    if(off_move_active && off_move == NULL_MOVE) {
        int off_col = 0; // pick column furthest from tiger
        if(current.get_tiger().location.col < 4) {
            off_col = NUM_COL - 1;
        }

        if(off_col == prev_off_col) {
            if(abs(current.get_tiger().location.col - 4) < 2) {
                off_col = NUM_COL - 1 - off_col;
                prev_off_col = off_col;
            }
        }

        int row = -1;
        // pick lowest token in off_col
        auto r = current.rows_in_col(off_col).begin();
        while(r != current.rows_in_col(off_col).end()) {
            row = max(*r, row);
            ++r;
        }

        // create off move and set to_do to it
        Token_t man = make_man(make_point(row, off_col));
        off_move = make_move_in_direction(man, DOWN);
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
        prev_off_col = to_do.destination.col;
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
                        delete [] moves_to_do[i];
                    }
                    delete [] moves_to_do;
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
                        swap(moves_to_do[c][0], moves_to_do[c][1]);
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
        if(desired.size() > 0) {
            Transition t(&current, &desired, &game, back_row, make_pair(c, c+3),
                         make_pair(front_row, back_row + 1));
            int min_req = INFTY;
            int max_req = -1;
            for(int i = 0; i < 6; ++i) {
                max_req = max(max_req, req_moves[i]);
                if(req_moves[i] > 0) {
                    min_req = min(req_moves[i], min_req);
                }
            }
            move_ready = t.find_path_to_state(min(4,max_req), min_req);
            if(move_ready) {
                path = t.get_path();
            }
        }
    }


    /// look for moves which move forward individual columns
    /*
    for(int c = 0; c < NUM_COL && !move_ready; ++c) {
        if(c == off_col) {
            continue;
        }
        desired.clear();
        path.clear();

        // desired states are those which do all moves_to_do in column
        // c
        int req_moves = 0;
        Move_t mv;
        Hash_val desired_hash = hash_locs(current, back_row);
        for(int k = 0; k < 2; ++k) {
            mv = moves_to_do[c][k];
            if(mv != NULL_MOVE) {
                ++req_moves;
                current.do_move(mv);
                desired_hash = next_hash(mv, desired_hash, back_row);
            }
        }
        // if the suggested desired state is secure, add it to the set
        // of desired states
        if(secure(&current, &game, off_move) && req_moves > 0) {
            desired.insert(desired_hash);
        }
        else {
            req_moves = 0;
        }

        // undo moves
        for(int k = 1; k >= 0; --k) {
            mv = moves_to_do[c][k];
            if(mv != NULL_MOVE) {
                current.do_move(-mv);
            }
        }

        // create transition, and have it look for a path to the desired states
        if(desired.size() > 0) {
            int left_bd = max(0, c-1);
            int r_bd = min(NUM_COL, c + 1);
            Transition t(&current, &desired, &game, back_row, make_pair(left_bd, r_bd),
                         make_pair(front_row, back_row + 1));
            move_ready = t.find_path_to_state(2 * req_moves);
            if(move_ready) {
                path = t.get_path();
            }
        }
    }
    */

    // free memory
    for(int c = 0; c < NUM_COL; ++c) {
        delete [] moves_to_do[c];
    }
    delete [] moves_to_do;
}


Move_t Smart_Mover::fail_safe(Move_t suggested) {
    // first call safety failsafe
    Move_t to_do = safety_fail_safe(suggested);
    if(to_do != NULL_MOVE) {
        suggested = to_do;
    }
    // now ensure validity
    if(!game.isValidMove(current, suggested)) {
        // if move not valid, look for a valid safe move
        bool fail_safe_found = false;
        // try each direction of moves and see if finds one that is valid
        // with no immediage jump
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        for(int i = 0; i < 8 && !fail_safe_found; ++i) {
            for(int c = 0; c < NUM_COL && !fail_safe_found; ++c) {
                auto r = current.rows_in_col(c).begin();
                while(r != current.rows_in_col(c).end() && !fail_safe_found) {
                    Token_t man = make_man(make_point(*r,c));
                    Move_t mv = make_move_in_direction(man, dirs[i]);
                    if(game.isValidMove(current, mv) && mv.destination.row >= front_row) {
                        current.do_move(mv);
                        if(!tiger_can_jump(&current, &game)) {
                            fail_safe_found = true;
                            to_do = mv;
                        }
                        current.do_move(-mv);
                    }
                    ++r;
                }
            }
        }
        // if cannot find a safe valid move, just try to find a valid move
        for(int i = 0; i < 8 && !fail_safe_found; ++i) {
            for(int c = 0; c < NUM_COL && !fail_safe_found; ++c) {
                auto r = current.rows_in_col(c).begin();
                while(r != current.rows_in_col(c).end() && !fail_safe_found) {
                    Token_t man = make_man(make_point(*r,c));
                    Move_t mv = make_move_in_direction(man, dirs[i]);
                    if(game.isValidMove(current, mv) && mv.destination.row >= front_row) {
                        fail_safe_found = true;
                        to_do = mv;
                    }
                    ++r;
                }
            }
        }
    }

    return to_do;
}


Move_t Smart_Mover::safety_fail_safe(Move_t suggested) {
    Move_t to_do = NULL_MOVE;

    current.do_move(suggested);
    bool can_jump = tiger_can_jump(&current, &game);
    current.do_move(-suggested);
    if(can_jump) {
        bool fail_safe_found = false;
        // try each direction of moves and see if can prevent the
        // capture
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        for(int i = 0; i < 8 && !fail_safe_found; ++i) {
            for(int c = 0; c < NUM_COL && !fail_safe_found; ++c) {
                auto r = current.rows_in_col(c).begin();
                while(r != current.rows_in_col(c).end() && !fail_safe_found) {
                    Token_t man = make_man(make_point(*r,c));
                    Move_t mv = make_move_in_direction(man, dirs[i]);
                    if(game.isValidMove(current, mv) && mv.destination.row >= front_row) {
                        current.do_move(mv);
                        if(!tiger_can_jump(&current, &game)) {
                            fail_safe_found = true;
                            to_do = mv;
                        }
                        current.do_move(-mv);
                    }
                    ++r;
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

    // if no move found still, try an off move
    if(!move_ready && !to_tiger_cage) {
        off_move_active = !off_move_active;
    }
    // undo off move if to tiger cage
    if(to_tiger_cage && off_move_active) {
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
            if(off_move_active) {
                off_move_active = false;
                off_move = NULL_MOVE;
            }
            else {
                off_move_active = true;
                off_move = -to_do;
            }
        }
        move_ready = false;
        this->path.clear();
        this->desired.clear();
        this->special_moves.clear_moves();
        to_do = fs;
    }

    // do move in this object
    current.do_move(to_do);

    return to_do;
}


Smart_Mover::Smart_Mover(const State& s):Men_Mover(s){
    back_row = NUM_ROW - 1;
    front_row = back_row - 2;
    special_moves = Specific_Move_Handler(&current, &game);

    // about next mvoe
    move_ready = false;
    off_move_ready = false;
    path.clear();
    desired.clear();

    // about off move
    prev_off_col = 0;
    off_move_active = false;
    off_move = NULL_MOVE;

    // whether or not to tiger
    to_tiger_cage = false;
}
