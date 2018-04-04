#ifndef PLAY_TIGER_H_INCLUDED
#define PLAY_TIGER_H_INCLUDED

#include "Man_Mover.h"
#include "Board.h"

class Play_Tiger{
    private:
        State game_state;
        Board brd;
        Man_Mover men_response;
    public:
        Play_Tiger() {}
        void print_board(ostream& out = cout);
        void play(istream& in = cin);
};


#endif // PLAY_TIGER_H_INCLUDED
