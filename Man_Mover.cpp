#include "Man_Mover.h"

Man_Mover::Man_Mover() {
    back_row = 12; // lowest row on board
    executing = false;
    greedy_phase = true;
    prev_state = SAME_ROW;
    off_move_active = false;
    off_move.destination = BAD_LOC;
    path.clear();
    desired.clear();
}

Move_t Man_Mover::next_move(State& s) {
    /// execution phase
    if(executing) {
        // determine if board in danger because of off_move
        Move_t off_move_capture;
        off_move_capture.token = s[0];
        off_move_capture.destination = off_move.token.location;
        // if board is in danger because of off move, fix off move
        if(brd.isValid(s, off_move_capture)) {
            off_move_active = false;
        }
        // if fixing an off move, do so
        if(!off_move_active && !(off_move.destination == BAD_LOC)) {
            Move_t new_move;
            // put reverse of off move in new_move
            new_move.token = off_move.token;
            new_move.token.location = off_move.destination;
            new_move.destination = off_move.token.location;
            // put BAD_LOC in off_move.destination to indicate no OFF_MOVE
            // is active
            off_move.destination = BAD_LOC;
            off_move_active = false;

            return new_move;
        }

        // if executing an off move, do so
        if(off_move_active && off_move.destination == BAD_LOC) {
            // assert tiger is first
            assert(s[0].color == RED);
            Move_t new_move;
            new_move.destination = make_point(-1,-1);
            new_move.token.location = make_point(-1,-1);
            // if tiger on left, move rightmost back token back one, else
            // move leftmost token back one
            int target_col = 0;
            if(s[0].location.col < 4) {
                target_col = 8;
            }
            for(auto i = s.begin(); i != s.end(); ++i) {
                // if token on target column and is moving back
                // at least as far as any other man can on this column,
                // set it as the new move
                if(i ->location.col == target_col){
                    if(new_move.destination.row <= i->location.row) {
                        new_move.token = *i;
                        new_move.destination = i->location;
                        new_move.destination.row += 1;
                    }
                }
            }

            return new_move;
        }

        // pretend off_move is not active, then see if otherwise at desired
        // state
        if(off_move_active) {
            for(auto i = s.begin(); i != s.end(); ++i) {
                if(i->location == off_move.destination) {
                    i->location = off_move.token.location;
                }
            }
            // if so, undo the off move
            if(man_pos_in(s, this->desired)) {
                off_move_active = false;
                return next_move(s);
            }
        }
        // if at desired state, go to next phase
        if(man_pos_in(s, this->desired)) {
            executing = false;
            path.clear();
            desired.clear();
            return next_move(s);
        }
        // else, continue executing
        assert(path.find(s) != path.end());
        return path[s];
    }

    /// greedy phase, look for safe forward movements
    if(greedy_phase) {
        int off_col = -3; // column off move done in
        // if off move is active, pretend like it was never done
        if(off_move_active) {
            for(auto i = s.begin(); i != s.end(); ++i) {
                if(i->location == off_move.destination) {
                    i->location = off_move.token.location;
                    off_col = i->location.col;
                }
            }
        }

        State_Transition transition = empty_transition(); // used to look for
                                     //transitions from current state to various
                                     // desired states
        Men_Move_F forward_mover(back_row); // used to define valid
                                                  // movements for men
        Tiger_Move tiger_mover; // used to define valid movements for
                                      // tiger
        bool path_found = false; // true iff a greedy forward path is found

        /// look for movements that move up two columns forward
        transition.current = s;
        transition.max_moves = 4;
        transition.back_row = this->back_row;
        path_found = try_sets_of_columns(transition, forward_mover, tiger_mover,
                                         off_col, 3, 2);
        cout << (int) path_found << endl;

        /// look for movements that move individual columns forward if no others
        /// found
        if(!path_found) {
            transition.current = s;
            transition.max_moves = 2;
            transition.back_row = this->back_row;
            transition.found_states.clear();
            transition.to_move.clear();
            transition.desired.clear();
            path_found = try_sets_of_columns(transition, forward_mover, tiger_mover,
                                         off_col);
        }


        // - if no path is found, and an off move hasn't been done, try one by
        //   passing to execution phase, but leaving greedy on
        //      - if an off move has been done, turn off greedy, and pass to
        //        executing to fix off move
        if(!path_found) {
            if(!off_move_active) {
                // perform off_move
                off_move_active = true;
            }
            else {
                // fix off move using execution phase
                greedy_phase = false;
            }
        } // if a path has been found, use it
        else {
            this->path = transition.path;
            this->desired = transition.desired;
        }
        // pass to execution phase to either perform/fix an off move or perform
        // a greedy move if one is found
        executing = true;
        return next_move(s);
    }

    /// check to see if in a defined state
    vector<int> number_in_col(9,0);
    vector<int> number_in_row(13,0);
    for(auto i = s.begin(); i != s.end(); ++i) {
        if(i->color != RED) {
            number_in_row[i->location.row]++;
            number_in_col[i->location.col]++;
        }
    }
    bool two_in_each_column = true;
    for(unsigned int i = 0; i < number_in_col.size(); ++i) {
        if(number_in_col[i] != 2) {
            two_in_each_column = false;
        }
    } // if 2 in each column, see if in any defined state and if so,
      // set the back row
    bool in_defined_state = false;
    if(two_in_each_column) {
        for(unsigned int i = 0; i < number_in_row.size() - 1; ++i) {
            if(number_in_row[i + 1] == 9) {
                if(number_in_row[i] == 9) {
                    back_row = i;
                    prev_state = SAME_ROW;
                    in_defined_state = true;
                }
                else if(number_in_row[i] == 8) {
                    if(i < number_in_row.size() - 2 && number_in_row[i+2] == 1){
                        State t = s;
                        for(auto j = t.begin(); j != t.end(); ++j) {
                            if(j->color != RED) {
                                if(j->location.row <= (int)i){
                                    j->location.row -= 1;
                                }
                            }
                        }
                        if(secure_from_above(t, i, this->brd)) {
                            prev_state = ONE_BEHIND;
                            back_row = i + 1;
                            in_defined_state = true;
                        }
                    }
                }
            }
            else if(number_in_row[i] == 8 && number_in_row[i+1] == 8) {
               if(i < number_in_row.size() - 3) {
                   if(number_in_row[i+2] == 1 && number_in_row[i+3] == 8){
                       if(secure_from_above(s, i + 1, this->brd)) {
                           prev_state = TWO_BEHIND;
                           back_row = i + 1;
                           in_defined_state = true;
                       }
                   }
               }
            }
        }
    }
    /// if in a defined state and not to last row, pass to greedy phase
    if(in_defined_state && back_row > 5) {
        greedy_phase = true;
        return next_move(s);
    }


    /// if not in a defined state, try for special moves on sets of
    /// adjacent four columns
    State_Transition transition = empty_transition();
    transition.current = s;
    transition.max_moves = 8;
    transition.back_row = back_row;
    Men_Move_FLR man_mover(back_row); // used to define valid
                                              // movements for men
    Tiger_Move tiger_mover; // used to define valid movements for
                                  // tiger
    assert(off_move_active == false);
    bool special_move_found = try_sets_of_columns(transition, man_mover,
                                                  tiger_mover, -1, 4, 3);
    assert(special_move_found);
    executing = true;
    path = move(transition.path);
    desired = move(transition.desired);
    return next_move(s);

}
