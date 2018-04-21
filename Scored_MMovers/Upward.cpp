//
// Created by EDWARDSAI on 4/21/2018.
//

#include "Upward.h"

void Upward::genScores(int ** scores) {
    int mod = 0;
    for(int r = 0; r < 13; r++){
        for(int c = 0; c < 9; c++){
            scores[12-r][c] += mod;
        }
        mod += 12-r;
    }
}
