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
    /*       Men_Mover(const State& s)
     *
     * description: creates a Men_Mover object, sets current to s
     * return: none
     * precondition: s is a valid State
     * postcondition: a Men_Mover object is created
     */
    Men_Mover(const State &s);

    /*       next_move(State s)
     *
     * description: returns the Move_t which the men should perform in the
     *              given state
     * return: Move_t
     * precondition: s is a valid State
     * postcondition: this current is set to s
     */
    virtual Move_t next_move(State s);

    /*       ~Men_Mover()
     *
     * description: destructor
     * return: none
     * precondition: none
     * postcondition: this object is deleted
     */
    virtual ~Men_Mover() = default;
protected:
    /*       execute_move()
     *
     * description: returns the Move_t which the men should perform given
     *              that current is the current state
     * return: Move_t
     * precondition: current is a valid State
     * postcondition: none
     */
    virtual Move_t execute_move() = 0;
};



#endif // MEN_MOVER_H_INCLUDED
