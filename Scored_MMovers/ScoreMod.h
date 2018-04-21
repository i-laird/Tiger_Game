//
// Created by EDWARDSAI on 4/21/2018.
//

#ifndef TIGER_GAME_SCOREMOD_H
#define TIGER_GAME_SCOREMOD_H


#include "../game_runner.h"

class ScoreMod {
public:
    virtual void
    genScores(int **scores, GameRunner &game, Unordered_State &current) = 0;
};


#endif //TIGER_GAME_SCOREMOD_H
