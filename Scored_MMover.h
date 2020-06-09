//
// Created by Aidan on 4/18/2018.
//

#ifndef WHATISTHIS_SCORED_MMOVER_H
#define WHATISTHIS_SCORED_MMOVER_H

#include <vector>
#include "Men_Mover.h"
#include "Scored_MMovers/ScoreMod.h"
#include "constants.h"



class Scored_MMover : public Men_Mover {
private:
    std::vector<ScoreMod*> mods;
public:
    Scored_MMover(const State &s);
    virtual ~Scored_MMover();
    Scored_MMover* addMod(ScoreMod* mod);
private:
    int **genScores();
    int scoreDelta(int **scores, Move_t move);
protected:
    Move_t execute_move() override;
};


#endif //WHATISTHIS_SCORED_MMOVER_H
