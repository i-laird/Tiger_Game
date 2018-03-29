#ifndef MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
#define MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED

#include <vector>
#include <cassert>
#include "function_api.h"

using namespace std;

typedef map<Token_t, Point_t> State;

bool operator<(const Token_t& a, const Token_t& b) {
    return make_pair(a.location.row, a.location.col) <
                                     make_pair(b.location.row, b.location.col);
}

bool operator<(const State& a, const State& b) {
    if(a.size() != b.size()) {
        return a.size() < b.size();
    }
    else {
        for(auto i = a.begin(); i != a.end(); ++i){
            if(i->second != b[i->first]) {
                return i->second < b[i->first];
            }
        }
    }
    return false;
}

bool is_equal(const State& a, const State& b, const vector<Token_t>& to_check) {
    for(auto i = to_check.begin(); i != to_check.end(); ++i) {
        State::iterator a_ptr = a.find(*i), b_ptr = b.find(*i);
        if((a_ptr == a.end()) != (b_ptr == b.end())) {
            return false;
        }
        else if(a_ptr != a.end() && b_ptr != b.end()) {
            if(a_ptr->second != b_ptr->second) {
                return false;
            }
        }
    }
    return true;
}

struct State_Transition{
    State current, desired;
    set<State> found_states;
    vector<Token_t> to_move;
    map<State,Move_t> path;
    int max_moves;
    int cur_moves;
    int back_row;
};

// this function returns true if and only if there is a path of moves
// from the current state to the desired state which allows for the following
// guidelines:
//           - Every man move is valid as determined by the function operator()
//             in the class T
//           - For each tiger response to the move (as determined by operator()
//             in the class M), this function called on the new state returns
//             true, i.e. there is a path of moves from this new state to the
//             desired one following these guidelines
//           - There is no cycling (i.e. we never return to a previous state)
//           - The only tokens moved are those in to_move
// If true, once this function returns, path will record the moves to be taken
// at each state in order to guarantee the desired state from this current one
//
// NOTE: There are efficiency optimizations that could be made here:
//             - Only the tiger and the men in to_move need to be checked
//               for cycling board state as they are the only
//               ones who should be moving. (Note that the entire board
//               should still be passed in as T or M may need to look at other
//               tokens in order to determine validity)
//
template <class M, class T>
bool find_path_to_state(State_Transition& t, M& det_man_moves,
                                                            T& det_tiger_moves) {
    // if at max moves, return true if reached desired state, false else
    if(t.num_moves >= t.max_moves){
        if(is_equal(t.current, t.desired, t.to_move)) {
            return true;
        }
        else{
            return false;
        }
    }

    // if been here before, return false, cycling
    if(t.found_states.find(t.current) != t.found_states.end()) {
        return false;
    }
    else{
        t.found_states.insert(t.current);
    }

    // determine whether this state is any good, i.e. if there exists a
    // move which, no matter the tiger response, allows one to reach the
    // desired state
    bool any_good = false; // true iff path to desired state following above
                           // guidelines
    vector<Move_t> man_moves = det_man_moves(t.current, t.to_move);
    for(auto m = man_moves.begin(); m != man_moves.end() && !any_good; ++m){
        bool move_okay = true;
        Point_t prev_position = t.current[m->token]; // record position
        // perform move
        t.current[m->token] = m->destination;
        ++t.num_moves;
        // check each tiger response
        vector<Move_t> tiger_moves = det_tiger_moves(t.current, t.to_move);
        for(auto t = tiger_moves.begin(); t != tiger_moves.end() && m_okay; ++t) {
            Point_t prev_tiger_pos = t.current[t->token];
            t.current[t->token] = t->destination;
            if(!find_path_to_state(t, det_man_moves, det_tiger_moves)){
                m_okay = false;
            }
            t.current[t->token] = prev_tiger_pos;
        }
        // undo move
        --t.num_moves;
        t.current[m->token] = prev_position;
        // if move is okay, record it
        if(m_okay){
            any_good = true;
            t.path[t.current] = Move_t(*m);
        }
    }

    return any_good;
}


#endif // MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
