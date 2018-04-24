#include "Transition.h"


bool Transition::search_for_state(int max_moves, int num_moves) {
    Token_t tiger = current->get_tiger();
    // if already know result of this state return it (i.e. have
    // been to this state with this many or more moves remaining)
    pair<Hash_val, Token_t> ndx = make_pair(cur_hash_val, tiger);
    auto visited_ptr = visited.find(ndx);
    if(visited_ptr != visited.end()) {
        // if been here before with at least as many remaining moves and could
        // not get there, return false
        if(num_moves >= visited_ptr->second.first &&
           visited_ptr->second.second == false) {
            return visited_ptr->second.second;
        } // else if been here before with fewer remaining moves and could
          // get there, return true
        else if(num_moves <= visited_ptr->second.first &&
                visited_ptr->second.second == true){
            return visited_ptr->second.second;
        }
    }

    // see if at desired state
    bool desired_state_found = false;
    if(hash_vals->find(cur_hash_val) != hash_vals->end()) {
        desired_state_found = true;
    }


    // if at max moves or found a desired state, return
    if(desired_state_found || num_moves >= max_moves){
        visited[ndx] = make_pair(num_moves, desired_state_found);
        return desired_state_found;
    }

    // determine whether this state is any good, i.e. if there exists a
    // move which, no matter the tiger response, allows one to reach the
    // desired state
    bool any_good = false; // true iff path to desired state following above
                           // guidelines

    // determine men to move
    vector<Token_t> to_move;
    for(int c = left_r_bds.first; c < left_r_bds.second; ++c) {
        for(auto r = current->rows_in_col(c).begin();
                                    r != current->rows_in_col(c).end(); ++r) {
            to_move.push_back(make_man(make_point(*r,c)));
        }
    }

    // for each man
    for(unsigned int i = 0; i < to_move.size() && !any_good; ++i) {
        Token_t man = to_move[i];
        auto man_moves = game->validMoves(*current, man);
        for(int m = 0; m <  man_moves.second.second && !any_good; ++m){
            bool m_okay = true; // true iff this move is a good response to
                                // the current state
            Move_t mv = make_move(man, man_moves.first[m]);
            // don't move out of bounds
            if(mv.destination.row < low_high_bds.first ||
               mv.destination.row >= low_high_bds.second ||
               mv.destination.col < left_r_bds.first ||
               mv.destination.col >= left_r_bds.second) {
                m_okay = false;
                /// DO THIS BETER ????
                continue;
           }
            // perform move
            cur_hash_val = next_hash(mv, cur_hash_val, back_row);

            current->do_move(mv);
            // if tiger can capture after move, this is a bad move
            if(tiger_can_jump(current, game)) {
                visited.insert(make_pair(make_pair(cur_hash_val, tiger), make_pair(0, false)));
                m_okay = false;
            }
            // check each tiger response
            auto tiger_moves = game->validMoves(*current, tiger);
            for(auto t = 0; t < tiger_moves.second.second && m_okay; ++t) {
                // move tiger
                Move_t t_mv = make_move(tiger, tiger_moves.first[t]);
                current->do_move(t_mv);
                if(!search_for_state(max_moves, num_moves + 1)) {
                    m_okay = false;
                }
                // undo tiger move
                current->do_move(-t_mv);
            }
            // free memory
            if(tiger_moves.second.first) {
                delete[] tiger_moves.second.first;
                tiger_moves.second.first = nullptr;
            }
            if(tiger_moves.first) {
                delete[] tiger_moves.first;
                tiger_moves.first = nullptr;
            }
            // undo move
            current->do_move(-mv);
            cur_hash_val = next_hash(-mv, cur_hash_val, back_row);
            // if move is okay, record it
            if(m_okay){
                any_good = true;
                path[ndx] = mv;
            }
        }
        // free memory
        if(man_moves.second.first) {
            delete[] man_moves.second.first;
            man_moves.second.first = nullptr;
        }
        if(man_moves.first) {
            delete[] man_moves.first;
            man_moves.first = nullptr;
        }
    }

    visited[ndx] = make_pair(num_moves, any_good); // record answer
    return any_good;
}



Transition::Transition(Unordered_State* c, set<Hash_val>*hash_vals, GameRunner*game,
                   int back_row, pair<int,int> left_right_bds,
                   pair<int,int> low_high_bds) {

    this->current = c;
    this->hash_vals = hash_vals;
    this->game = game;
    this->back_row = back_row;
    this->cur_hash_val = hash_locs(*c, back_row);
    this->left_r_bds = left_right_bds;
    this->low_high_bds = low_high_bds;
    this->path.clear();
    this->visited.clear();
}


bool Transition::find_path_to_state(int max_moves, int minimum_max_moves) {
    // see if can reach a desired state "without moving" this time
    // this prevents degenerating into cycles
    bool found = false;
    for(int i = max_moves - max(1, minimum_max_moves); i >= 0 && !found; --i) {
        this->visited.clear();
        this->path.clear();
        if(search_for_state(max_moves, i)) {
            found = true;
        }
    }
    return found;
}


map<pair<Hash_val,Token_t>, Move_t> Transition::get_path() {
    return this->path;
}
