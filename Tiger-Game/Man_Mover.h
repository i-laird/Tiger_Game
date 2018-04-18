#ifndef MAN_MOVER_H_INCLUDED
#define MAN_MOVER_H_INCLUDED

#include "Board.h"

class Man_Mover{
    public:
        virtual Move_t next_move(State s)=0;
};

#endif // MAN_MOVER_H_INCLUDED
