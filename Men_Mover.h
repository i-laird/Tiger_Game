#include <iostream>
#include "Transition.h"
#include "Specific_Move_Handler.h"
#include "testing_util.h"

#ifndef MEN_MOVER_H_INCLUDED
#define MEN_MOVER_H_INCLUDED


class Men_Mover {
protected:
    // about current state
    GameRunner game;
    Unordered_State current;
public:
    Men_Mover(const State &s);
    virtual ~Men_Mover() = default;

    virtual Move_t next_move(State s);
protected:
    virtual Move_t execute_move() = 0;
};



#endif // MEN_MOVER_H_INCLUDED
