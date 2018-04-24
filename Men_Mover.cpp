#include "Men_Mover.h"

Men_Mover::Men_Mover(const State &s) : current(s) {}

Move_t Men_Mover::next_move(State s) {
    this->current = Unordered_State(s);
    return execute_move();
}