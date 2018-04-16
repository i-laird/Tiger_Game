#include <iostream>

#include "Unorderded_State.h"
#include <queue>

using namespace std;

#ifndef SPECIFIC_MOVE_HANDLER_H_INCLUDED
#define SPECIFIC_MOVE_HANDLER_H_INCLUDED

class Specific_Move_Handler {
    private:
        const Unordered_State * current;
        queue<Move_t> response;
        int front_row, back_row;

        bool one_col_two_back();
    public:
        Specific_Move_Handler(const Unordered_State* = nullptr);
        void set_front_back_row(int front_row, int back_row);
        bool handle_special_case();
        Move_t get_move();
};


#endif // SPECIFIC_CASE_HANDLER_H_INCLUDED
