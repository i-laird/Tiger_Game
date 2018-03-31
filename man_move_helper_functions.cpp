#include "man_move_helper_functions.h"

bool operator<(const Token_t& a, const Token_t& b) {
    return a.location < b.location;
}

bool operator<(const Point_t& a, const Point_t& b) {
    return make_pair(a.row, a.col) < make_pair(b.row, b.col);
}

template <class M, class T>
bool find_path_to_state(State_Transition& t, M& det_man_moves,
                                                            T& det_tiger_moves) {
    // if at max moves, return true if reached desired state, false else
    if(t.num_moves >= t.max_moves){
        if(t.current == t.desired) {
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
    vector<pair<int, Move_t> > man_moves = det_man_moves(t.current, t.to_move); // index to move
    for(auto m = man_moves.begin(); m != man_moves.end() && !any_good; ++m){
        bool m_okay = true; // true iff this move is a good response to
                            // the current state
        Point_t prev_position = t.current[m->first]; // record position
        // perform move
        t.current[m->first].location = m->second.destination;
        ++t.num_moves;
        // check each tiger response
        vector<pair<int, Move_t> > tiger_moves = det_tiger_moves(t.current, t.to_move);
        for(auto i = tiger_moves.begin(); i != tiger_moves.end() && m_okay; ++i) {
            assert(t.current[i->first].color == RED);
            Point_t prev_tiger_pos = t.current[i->first].location;
            // move tiger
            t.current[i->first].location = i->second.destination;
            if(!find_path_to_state(t, det_man_moves, det_tiger_moves)){
                m_okay = false;
            }
            // undo tiger move
            t.current[i->first].location = prev_tiger_pos;
        }
        // undo move
        --t.num_moves;
        t.current[m->first].location = prev_position;
        // if move is okay, record it
        if(m_okay){
            any_good = true;
            t.path[t.current] = Move_t(*m);
        }
    }

    return any_good;
}


Men_move_forward::Men_move_forward(){
    this->back_row = -1;
}

vector<pair<int, Move_t> > Men_move_forward::operator()(const State& s, const vector<int>& to_move){
    // find back_row if not already found
    if(back_row == -1) {
        assert(to_move.size() >= 1);
        back_row = s[to_move[0]].location.row;
        for(int i = 1; i < to_move.size(); ++i) {
            back_row = min(back_row, s[to_move[i]].location.row);
        }
    }
    set<Point_t> occupied; // set of occupied points
    Point_t tiger_pos; // tiger position
    for(auto i = s.begin(); i != s.end(); ++i) {
        occupied.insert(i->location);
        if(i->color == RED) {
            tiger_pos = i->location;
        }
    }

    vector<pair<int, Move_t> > suggested_moves;
    // for each man that might be moved
    for(int i = 0; i < to_move.size(); ++i) {
        Point_t new_location = s[i].location;
        new_location.row -= 1;
        // if new space unoccupied
        if(occupied.find(new_location) == occupied.end()){
            /// if tiger cannot jump anyone **** NEEDS TO BE IMPLEMENTED***
                Move_t new_move;
                new_move.destination = new_location;
                new_move.token = s[i];
                suggested_moves.push_back(make_pair(i, new_move));
        }
    }
    return suggested_moves;
}
