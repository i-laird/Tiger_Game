//
// Created by EDWARDSAI on 4/21/2018.
//

#include "Inward.h"

void Inward::genScores(int ** scores) {
    int mod = 0;
    for(int c = 0; c < 5; c++){
        for(int r = 0; r < 13; r++){
            scores[r][c] += mod;
            if(8-c != c){
                scores[r][8-c] += mod;
            }
        }
        mod += 4-c;
    }
}
