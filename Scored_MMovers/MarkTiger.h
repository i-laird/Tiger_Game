//
// Created by EDWARDSAI on 4/21/2018.
//

#ifndef TIGER_GAME_MARKTIGER_H
#define TIGER_GAME_MARKTIGER_H


#include "ScoreMod.h"

class MarkTiger : public ScoreMod {
public:
    void
    genScores(int **scores, GameRunner &game, Unordered_State &current) override;
};


#endif //TIGER_GAME_MARKTIGER_H
