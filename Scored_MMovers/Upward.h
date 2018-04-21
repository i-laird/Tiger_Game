//
// Created by EDWARDSAI on 4/21/2018.
//

#ifndef TIGER_GAME_UPWARD_H
#define TIGER_GAME_UPWARD_H


#include "ScoreMod.h"
#include "../game_runner.h"
#include "../Unordered_State.h"

class Upward : public ScoreMod {
public:
    void
    genScores(int **scores, GameRunner &game, Unordered_State &current) override;
};


#endif //TIGER_GAME_UPWARD_H
