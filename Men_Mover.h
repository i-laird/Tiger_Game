#include <iostream>
#include "Transition.h"
#include "Specific_Move_Handler.h"
#include "testing_util.h"

#ifndef MEN_MOVER_H_INCLUDED
#define MEN_MOVER_H_INCLUDED


class Men_Mover {
    private:
        // about current state
        GameRunner game;
        Unordered_State current;
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
        Men_Mover(const State& s);
        Move_t next_move(Move_t tiger_move);
};


#endif // MEN_MOVER_H_INCLUDED
