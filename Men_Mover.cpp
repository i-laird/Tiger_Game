#include "Men_Mover.h"

Men_Mover::Men_Mover(const State &s) : current(s) {}

Move_t Men_Mover::next_move(Move_t tiger_move) {
    // do tiger move
    this->current.do_move(tiger_move);
    return execute_move();
}