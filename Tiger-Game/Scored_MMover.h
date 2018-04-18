//
// Created by Aidan on 4/18/2018.
//

#ifndef WHATISTHIS_SCORED_MMOVER_H
#define WHATISTHIS_SCORED_MMOVER_H

#include <vector>
#include "Man_Mover.h"


class Scored_MMover : public Man_Mover {
private:
    Board brd;
protected:
    int **genScores(const State &vector1);
    int scoreDelta(int **scores, Move_t move);
public:
    Move_t next_move(State s);
};


#endif //WHATISTHIS_SCORED_MMOVER_H
