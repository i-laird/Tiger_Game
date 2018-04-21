//
// Created by EDWARDSAI on 4/21/2018.
//

#include "MarkTiger.h"

void MarkTiger::genScores(int **scores, GameRunner &game, Unordered_State &current) {
    scores[current.get_tiger().location.row][current.get_tiger().location.col] -= 1000;
}
