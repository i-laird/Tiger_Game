#ifndef MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
#define MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED

#include <list>
#include <map>
#include <cassert>

#include "Board.h"

using namespace std;

bool operator<(const Token_t & a, const Token_t & b);

Point_t make_point(int r, int c);

void p_board(ostream& out, const State& s);




// returns true iff the positions filled by men in
// s are the same as those filled by men in t
bool man_pos_in(const State& s, const set<State>& t);

// returns true iff there is no man who can be captured
// by a jump which moves downward from any tiger position, and no man
// at/above the back_row who can be captured by a left/right
// jump from any tiger position
//
// ASSUMES NO MEN ARE IN TIGER CAGE
bool secure_from_above(const State& s, int back_row, const Board& brd);

struct State_Transition{
    State current;
    set<State> desired;
    set<State> found_states;
    list<int> to_move;
    map<State, Move_t> path;
    int max_moves;
    int num_moves;
    int back_row;
};

State_Transition empty_transition();

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
                                                            T& det_tiger_moves);

bool tiger_can_jump(const State& s, const Board& brd);


class Men_Move_F{
    private:
        Board brd;
        int back_row;
    public:
        Men_Move_F(int br = -1);
        // Finds all forward moves using tokens in the State of the given indices
        // which are forward, and leave the tiger unable to capture anyone
        vector<pair<int, Move_t> >* operator()(State&, const list<int>&);
};


class Men_Move_FLR {
    private:
        Board brd;
        int back_row;
        int left_col;
        int right_col;
    public:
        Men_Move_FLR(int br = -1);
        // Finds all forward, left, and right moves using tokens in the State
        // of the given indices which leave the tiger unable to capture anyone
        vector<pair<int, Move_t> >* operator()(State&, const list<int>&);
};

class Tiger_Move{
    private:
        Board brd;
    public:
        Tiger_Move() {}
        vector<pair<int, Move_t> >* operator()(State&, const list<int>&);
};


// tries to move forward sets of adjacent columns of size num_forward
// by looking at sets of size num_cols
// such that the desired state is secure and each move is valid according to
// det_man_moves
//
// in t, max_moves remains unchanged
template <class M, class T>
bool try_sets_of_columns(State_Transition& t, M& det_man_moves,
                         T& det_tiger_moves, int off_col,
                         int num_cols = 1, int num_forward = 1);





template <class M, class T>
bool find_path_to_state(State_Transition& t, M& det_man_moves,
                                                            T& det_tiger_moves) {
    cout << "CALLING\n";
    // see if at desired state
    bool desired_state_found = false;
    if(man_pos_in(t.current, t.desired)) {
        desired_state_found = true;
    }
    // if at max moves or found a desired state, return
    if(desired_state_found || t.num_moves >= t.max_moves){
        cout << "final moves = " << t.num_moves << endl;
        return desired_state_found;
    }

    // if been here before, return false, cycling
    if(t.found_states.find(t.current) != t.found_states.end()) {
        cout << "final moves on cycle = " << t.num_moves << endl;
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
    vector<pair<int, Move_t> >* man_moves = det_man_moves(t.current, t.to_move); // index to move
    cout << "NUMBEr OF POSSIBLE MOVES = " << man_moves->size() << endl;
    for(auto m = man_moves->begin(); m != man_moves->end() && !any_good; ++m){
        bool m_okay = true; // true iff this move is a good response to
                            // the current state
        Point_t prev_position = t.current[m->first].location; // record position
        // perform move
        t.current[m->first].location = m->second.destination;
        ++t.num_moves;
        // check each tiger response
        vector<pair<int, Move_t> >* tiger_moves = det_tiger_moves(t.current, t.to_move);
        for(auto i = tiger_moves->begin(); i != tiger_moves->end() && m_okay; ++i) {
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
        delete tiger_moves; // free memory
        // undo move
        --t.num_moves;
        t.current[m->first].location = prev_position;
        // if move is okay, record it
        if(m_okay){
            any_good = true;
            t.path[t.current] = Move_t(m->second);
        }
    }
    delete man_moves; // free memory
    cout << "AT ANY_GOOD\n";
    return any_good;
}


template <class M, class T>
bool try_sets_of_columns(State_Transition& t, M& det_man_moves,
                         T& det_tiger_moves, int off_col, int num_cols,
                         int num_forward) {
    // ensure function call is valid
    assert(num_cols <= 9 && num_cols > 0);
    // if no off_col, set negative enough that never close to board
    if(off_col < 0) {
        off_col = -num_cols;
    }
    // to hold indices of tokens in column i
    vector<pair<int,int> > col_to_index(9, make_pair(-1,-1));
    for(unsigned int i = 0; i < t.current.size(); ++i) {
        // don't record tiger
        if(t.current[i].color == RED) {
            continue;
        }
        if(col_to_index[t.current[i].location.col].first == -1) {
            col_to_index[t.current[i].location.col].first = i;
        }
        else {
            // assert that at most two tokens are in any given column
            assert(col_to_index[t.current[i].location.col].second == -1);
            col_to_index[t.current[i].location.col].second = i;
        }
    }
    bool path_found = false; // true iff a path has been found
                             // to a desired state

    // look for movements that move columns forward
    vector<int> cols_to_move; // columns to move forward
    for(int i = 0; i < num_cols; ++i) {
        cols_to_move.push_back(i);
    }
    list<int> tokens_to_move; // indexes of tokens in columns
    for(int i = 0; i < num_cols; ++i) {
        // add the indexes of the tokens in the columns being
        // moved
        tokens_to_move.push_back(col_to_index[i].first);
        t.to_move.push_back(tokens_to_move.back());
        tokens_to_move.push_back(col_to_index[i].second);
        t.to_move.push_back(tokens_to_move.back());
    }
    t.desired.clear();
    t.found_states.clear();

    /// look at each set of num_cols columns and try to move each adjacent pair
    for(int i = 0; i < 9 - num_cols + 1 && !path_found; ++i){

        // do not move off_col
        if(abs(i - off_col) < num_cols) {
            continue;
        }
        // assert transition is unmoved
        assert(t.num_moves == 0);

        State s = t.current;
        vector<bool> moved_up(false, s.size());
        // move up first num_forward columns
        for(int j = 0; j < num_forward; ++j) {
            pair<int, int> col_ndx = col_to_index[cols_to_move[j]];
            int highest_row = s[col_ndx.first].location.row;
            highest_row = min(highest_row, s[col_ndx.second].location.row);
            // don't move too far ahead of back row and don't move off col
            if(highest_row >= t.back_row - 2) {
                if(col_ndx.first != off_col) {
                    moved_up[col_ndx.first] = true;
                    --s[col_ndx.first].location.row;
                }
                if(col_ndx.second != off_col) {
                    moved_up[col_ndx.second] = true;
                    --s[col_ndx.second].location.row;
                }
            }
        }

        Board brd;
        // for each num_forward size subset of the current num_col columns,
        // if those columns shifted up one forward leaves the board secure
        // from above, add it to the set of desired states
        for(int j = 0; j < num_cols - num_forward + 1; ++j) {
            // if result of move is secure, add to desired states
            if(secure_from_above(s, t.back_row, brd)) {
                t.desired.insert(s);
                p_board(cout, s);
            }

            // look at next possible desired state
            if(j < num_cols - num_forward) {
                // move down leftmost column if it was moved up
                pair<int,int> col_ndx = col_to_index[cols_to_move[j]];
                if(moved_up[col_ndx.first]) {
                    moved_up[col_ndx.first] = false;
                    ++s[col_ndx.first].location.row;
                }
                if(moved_up[col_ndx.second]) {
                    moved_up[col_ndx.second] = false;
                    ++s[col_ndx.second].location.row;
                }
                // move up next column on right
                col_ndx = col_to_index[cols_to_move[j + num_forward]];
                int highest_row = s[col_ndx.first].location.row;
                highest_row = min(highest_row, s[col_ndx.second].location.row);
                // don't move too far ahead of back row and don't move off col
                if(highest_row >= t.back_row - 2) {
                    if(col_ndx.first != off_col) {
                        moved_up[col_ndx.first] = true;
                        --s[col_ndx.first].location.row;
                    }
                    if(col_ndx.second != off_col) {
                        moved_up[col_ndx.second] = true;
                        --s[col_ndx.second].location.row;
                    }
                }
            }
        }

        // if there are desired states, see if they are reachable
        if(t.desired.size() > 0) {
            cout << (int) path_found << " --> ";
            path_found = find_path_to_state(t, det_man_moves,
                                                              det_tiger_moves);
            cout << (int) path_found << endl;
        }

        // move to next group of columns
        for(auto i = cols_to_move.begin(); i != cols_to_move.end(); ++i) {
            ++(*i);
        }
        // remove leftmost column from to_move list
        tokens_to_move.pop_front();
        tokens_to_move.pop_front();
        t.to_move.pop_front();
        t.to_move.pop_front();
        // add rightmost column to to_move list if not done
        if(cols_to_move.back() < 9){
            tokens_to_move.push_back(col_to_index[cols_to_move.back()].first);
            t.to_move.push_back(tokens_to_move.back());
            tokens_to_move.push_back(col_to_index[cols_to_move.back()].second);
            t.to_move.push_back(tokens_to_move.back());
        }
        t.found_states.clear();
    }
    t.to_move.clear();

    return path_found;
}


#endif // MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
