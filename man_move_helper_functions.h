#ifndef MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
#define MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED

#include <vector>
#include <set>
#include <map>
#include <cassert>
#include "function_api.h"

using namespace std;

typedef vector<Token_t> State;

bool operator<(const Token_t& a, const Token_t& b);

bool operator<(const Point_t& a, const Point_t& b);

struct State_Transition{
    State current, desired;
    set<State> found_states;
    vector<int> to_move;
    map<State, Move_t> path;
    int max_moves;
    int num_moves;
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

class Men_Move_F{
    private:
        int back_row;
    public:
        Men_Move_F(int br = -1);
        // Finds all forward moves using tokens in the State of the given indices
        // which are forward, and leave the tiger unable to capture anyone
        //
        // NOTE: CHECKS THAT MOVES DO NOT HIT ANOTHER TOKEN, THAT THEY DO NOT
        //       PASS back_row - 2 IN ROW, HOWEVER NO OTHER CHECKS FOR VALIDITY
        //       ARE PERFORMED
        vector<pair<int, Move_t> >* operator()(const State&, const vector<int>&);
};


class Men_Move_FLR {
    private:
        int back_row;
        int left_col;
        int right_col;
    public:
        Men_Move_FLR(int br = -1, int left_c = -1, int right_c = -1);
        // Finds all forward, left, and right moves using tokens in the State
        // of the given indices which leave the tiger unable to capture anyone
        //
        // NOTE: CHECKS THAT MOVES DO NOT HIT ANOTHER TOKEN, THAT THEY STAY
        //       WITHIN [left_col, right_col] AND THAT THEY DO NOT PASS
        //       back_row - 2 IN ROW, HOWEVER NO OTHER CHECKS FOR VALIDITY
        //       ARE PERFORMED
        vector<pair<int, Move_t> >* operator()(const State&, const vector<int>&);
};

// - SAME_ROW indicates all columns are on same two adjacent rows
// - ONE_BEHIND indicates all columns are on same two
//   adjacent rows except for one column, which is one row behind
// - TWO_BEHIND indicates all columns are on same two adjacent rows
//   except for one column, which is two rows behind
enum Defined_State {SAME_ROW, ONE_BEHIND, TWO_BEHIND};



#endif // MAN_MOVE_HELPER_FUNCTIONS_H_INCLUDED
