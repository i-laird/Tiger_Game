//
// Created by EDWARDSAI on 4/21/2018.
//

#ifndef TIGER_GAME_UPWARD_H
#define TIGER_GAME_UPWARD_H


#include "ScoreMod.h"

class Upward : public ScoreMod {
public:
    void genScores(int ** scores) override;
};


#endif //TIGER_GAME_UPWARD_H
