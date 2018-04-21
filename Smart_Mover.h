#ifndef TIGER_GAME_SMART_MOVER_H
#define TIGER_GAME_SMART_MOVER_H

#include <iostream>
#include "Transition.h"
#include "Specific_Move_Handler.h"
#include "testing_util.h"
#include "Men_Mover.h"

class Smart_Mover : public Men_Mover {
private:
    // about current state
    int front_row;
    int back_row;

    // about next move
    bool move_ready;
    map<pair<Hash_val,Token_t>, Move_t> path;
    set<Hash_val> desired;

    Specific_Move_Handler special_moves;

    // about off move
    bool off_move_ready;
    int off_move_active;
    Move_t off_move;

    // whether or not to tiger cage
    bool to_tiger_cage;

    void determine_rows();
    Move_t** find_moves_to_do();

    Move_t off_move_handling();
    void search_for_state();

public:
    Smart_Mover(const State& s);

protected:
    Move_t execute_move() override;
};


#endif //TIGER_GAME_SMART_MOVER_H
