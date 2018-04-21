//
// Created by Aidan on 4/18/2018.
//

#ifndef WHATISTHIS_SCORED_MMOVER_H
#define WHATISTHIS_SCORED_MMOVER_H

#include <vector>
#include "Men_Mover.h"


class Scored_MMover : public Men_Mover {
protected:
public:
    Scored_MMover(const State &s);

protected:
    int **genScores();
    int scoreDelta(int **scores, Move_t move);
protected:
    Move_t execute_move() override;
};


#endif //WHATISTHIS_SCORED_MMOVER_H
