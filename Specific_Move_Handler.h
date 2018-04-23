#include <iostream>

#include "Unordered_State.h"
#include "game_runner.h"
#include "men_util.h"
#include <queue>

using namespace std;

#ifndef SPECIFIC_MOVE_HANDLER_H_INCLUDED
#define SPECIFIC_MOVE_HANDLER_H_INCLUDED

class Specific_Move_Handler {
    private:
        Unordered_State * current;
        GameRunner* g;
        queue<Move_t> response;
        int front_row, back_row;
        Move_t off_move;

        bool one_col_two_back();
        bool three_by_diag();
        bool near_top_finish_three_by_diag();
        bool lagging_col();
        bool try_switch();
    public:
        Specific_Move_Handler(Unordered_State* = nullptr, GameRunner* g = nullptr);
        void set_front_back_row(int front_row, int back_row);
        bool handle_special_case(Move_t off_move);
        void clear_moves();
        Move_t get_move();
};


#endif // SPECIFIC_CASE_HANDLER_H_INCLUDED
