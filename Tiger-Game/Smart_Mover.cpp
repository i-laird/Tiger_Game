//
// Created by Aidan on 4/18/2018.
//

#include "Smart_Mover.h"

Smart_Mover::Smart_Mover() {
    back_row = 12; // lowest row on board
    executing = false;
    greedy_phase = true;
    prev_state = SAME_ROW;
    special_move = false;
    special_moves_to_do = 0;
    off_move_active = false;
    off_move_index = -1;
    off_move.destination = BAD_LOC;
    path.clear();
    desired.clear();
}

Move_t Smart_Mover::next_move(State s) {
    /// execution phase
    if(executing) {
        set<Point_t> men_locations;
        for(auto i = s.begin(); i != s.end(); ++i) {
            if(i->color == BLUE) {
                men_locations.insert(i->location);
            }
        }
        // determine if board in danger because of off_move
        if(off_move_active){
            Move_t off_move_capture;
            off_move_capture.token = s[0];
            off_move_capture.destination = off_move.token.location;
            // if board is in danger because of off move, fix off move
            if(brd.isValid(s, off_move_capture)) {
                off_move_active = false;
            }
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
            off_move_index = -1;

            return new_move;
        }

        // if executing an off move, do so
        if(off_move_active && off_move.destination == BAD_LOC) {
            // assert tiger is first
            assert(s[0].color == RED);
            off_move.destination = make_point(-1,-1);
            off_move.token.location = make_point(-1,-1);
            // if tiger on left, move rightmost back token back one, else
            // move leftmost token back one
            int target_col = 0;
            if(s[0].location.col < 4) {
                target_col = 8;
            }
            for(unsigned int i = 0; i < s.size(); ++i) {
                // if token on target column and is moving back
                // at least as far as any other man can on this column,
                // set it as the new move
                if(s[i].location.col == target_col){
                    if(off_move.destination.row <= s[i].location.row) {
                        off_move.token = s[i];
                        off_move.destination = s[i].location;
                        off_move.destination.row += 1;
                        off_move_index = i;
                    }
                }
            }
            executing = false;
            return off_move;
        } // if off_move fixed by special move, mark it
        else if(off_move_active && !(s[off_move_index].location == off_move.destination)){
            off_move_active = false;
            off_move.destination = BAD_LOC;
            off_move_index = -1;
        }

        // if working towards a desired state
        if(this->desired.size() > 0) {
            // if off_move is active, pretend off_move is not active,
            // then see if otherwise at desired state
            set<Point_t> no_off = men_locations;
            if(off_move_active) {
                no_off.erase(off_move.destination);
                no_off.insert(off_move.token.location);
                // if so, undo the off move
                if(desired.find(no_off) != desired.end()) {
                    off_move_active = false;
                    return next_move(s);
                } // else, take next move as off_move is not present
                else {
                    s[off_move_index].location = off_move.token.location;
                }
            }// if no off move and at desired state, go to next phase
            else if(desired.find(men_locations) != desired.end()) {
                executing = false;
                path.clear();
                desired.clear();
                return next_move(s);
            }
            // if not at desired state and no off_move, continue executing
            assert(path.find(make_pair(s[0], men_locations)) != path.end());
            return path[make_pair(s[0], men_locations)];
        } // if working on special move
        else if(special_move) {
            // if special move finished, clean up path
            if(path.find(make_pair(s[0], men_locations)) == path.end() ||
               special_moves_to_do <= 0){
                special_move = false;
                special_moves_to_do = 0;
                path.clear();
            }// else continue executing it
            else {
                --special_moves_to_do;
                Move_t result = path[make_pair(s[0], men_locations)];
                // see if special move can end early
                if(special_moves_to_do > 0) {
                    bool moved = false;
                    for(auto i = s.begin(); i != s.end() && !moved; ++i) {
                        if(*i == result.token) {
                            moved = true;
                            i->location = result.destination;
                        }
                    }
                    if(secure_from_above(s, back_row, brd)){
                        special_move = false;
                        special_moves_to_do = 0;
                        path.clear();
                    }
                }
                return result;
            }
        }
        else {
            cout << "RIGHT HERE\n";
            executing = false;
        }
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
        Men_Move_F forward_mover(brd, back_row); // used to define valid
        // movements for men
        Tiger_Move tiger_mover(brd); // used to define valid movements for
        // tiger
        bool path_found = false; // true iff a greedy forward path is found

        /// look for movements that move up two columns forward
        transition.current = s;
        transition.max_moves = 4;
        transition.back_row = this->back_row;
        path_found = try_sets_of_columns(transition, forward_mover, tiger_mover,
                                         off_col, 3, 2);

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
                cout << "EXECUTING OFF MOVE";
                off_move_active = true;
            }
            else {
                // fix off move using execution phase
                cout << "ENDING GREEDY PHASE\n";
                off_move_active = false;
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
    for(auto i = number_in_row.begin(); i != number_in_row.end(); ++i) {
        cout << *i << " ";
    }cout << endl;
    bool two_in_each_column = true;
    for(unsigned int i = 0; i < number_in_col.size(); ++i) {
        if(number_in_col[i] != 2) {
            two_in_each_column = false;
        }
    } // if 2 in each column, see if in any defined state and if so,
    // set the back row
    bool in_defined_state = false;
    if(two_in_each_column) {
        cout << "CHECKING FOR STATE\n";
        for(unsigned int i = 0; i < number_in_row.size() - 1 && !in_defined_state; ++i) {
            if(number_in_row[i + 1] == 9) {
                if(number_in_row[i] == 9) {
                    back_row = i + 1;
                    prev_state = SAME_ROW;
                    in_defined_state = true;
                }
                else if(number_in_row[i] == 8) {
                    if(i < number_in_row.size() - 2 && number_in_row[i+2] == 1){
                        //back_row = i + 1;
                        //prev_state = ONE_BEHIND;
                        //in_defined_state = true;
                        State t = s;
                        int behind_col = -1;
                        // move forward front line of t
                        for(auto j = t.begin(); j != t.end(); ++j) {
                            if(j->color == BLUE) {
                                if(j->location.row == (int)i){
                                    j->location.row -= 1;
                                }
                                else if(j->location.row == (int) i + 2) {
                                    behind_col = j->location.col;
                                }
                            }
                        }
                        // move forward back line of t except for the col behind
                        for(auto j = t.begin(); j != t.end(); ++j) {
                            if(j->color != RED) {
                                if(j->location.row == (int)i + 1){
                                    if(j->location.col != behind_col) {
                                        j->location.row -= 1;
                                    }
                                }
                            }
                        }
                        // see if going from this ONE_BEHIND to TWO_BEHIND will be
                        // secure
                        if(secure_from_above(t, i, this->brd)) {
                            cout << "ONE_BEHIND\n";
                            prev_state = ONE_BEHIND;
                            back_row = i + 1;
                            in_defined_state = true;
                        }
                        else{cout << "UNSAFE\n\n";}
                        /*else {
                            prev_state = ONE_BEHIND_UNSAFE;
                            back_row = i + 1;
                            in_defined_state = true;
                        }*/
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

    /// if in a defined state and not to last row,
    /// pass to greedy phase
    if(in_defined_state && back_row > 5) {
        cout << "NEW BACK ROW = " << back_row << endl;
        greedy_phase = true;
        special_move = false;
        off_move_index = -1;
        off_move_active = false;
        off_move.destination = BAD_LOC;
        desired.clear();
        path.clear();
        return next_move(s);
    }


    /// if not in a defined state, try for
    /// special moves on sets of adjacent four columns
    // set up transition
    State_Transition transition = empty_transition();
    transition.current = s;
    transition.num_moves = 0;
    transition.back_row = back_row;
    for(auto i = s.begin(); i != s.end(); ++i) {
        if(i->color == BLUE){
            transition.men_locations.insert(i->location);
        }
    }
    for(int i = 1; i < 19; ++i) {
        transition.to_move.push_back(i);
    }
    int min_row_sum = MAX_ROW_SUM; // pick best special move
    transition.max_moves = MAX_SPECIAL_MOVES;
    transition.found_states.clear();
    transition.path.clear();

    Men_Move_FLR man_mover(brd, back_row); // used to define valid
    // movements for men
    Tiger_Move tiger_mover(brd); // used to define valid movements for
    // tiger
    cout << "TRYING SPECIAL MOVE\n";
    map<Orderless_State, int> found_states;
    Move_t prev_move;
    prev_move.destination = BAD_LOC;
    int result = minimize_row_sum(transition, found_states,
                                  prev_move, man_mover, tiger_mover);
    if(result < min_row_sum) {
        special_move = true;
        special_moves_to_do = MAX_SPECIAL_MOVES;
        min_row_sum = result;
        this->path = transition.path;
    }// if no special move found, perform an off move then try again
    else if(!off_move_active) {
        off_move_active = true;
    }
    else {
        cout << "NO OFF MOVE FOUND\n";
    }

    executing = true;
    return next_move(s);

}
