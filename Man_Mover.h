#ifndef MAN_MOVER_H_INCLUDED
#define MAN_MOVER_H_INCLUDED

#include "man_move_helper_functions.h"

const Point_t BAD_LOC = make_point(-1, -1);

// - SAME_ROW indicates all columns are on same two adjacent rows
// - ONE_BEHIND indicates all columns are on same two
//   adjacent rows except for one column, which is one row behind.
//   in addition, if all other columns were to be moved one row
//   forward, the board is secure
// - TWO_BEHIND indicates all columns are on same two adjacent rows
//   except for one column, which is two rows behind. In addition,
//   the board is secure.
enum Defined_State {SAME_ROW, ONE_BEHIND, TWO_BEHIND};

class Man_Mover{
    private:
        int back_row;
        bool executing;
        map<State, Move_t> path;
        set<State> desired;
        bool off_move_active;
        Move_t off_move;
        bool greedy_phase;
        Board brd;
        Defined_State prev_state;
    public:
        Man_Mover();
        Move_t next_move(State& s);
};

#endif // MAN_MOVER_H_INCLUDED
