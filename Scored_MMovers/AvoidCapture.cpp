//
// Created by EDWARDSAI on 4/21/2018.
//

#include "AvoidCapture.h"


void AvoidCapture::genScores(int **scores, GameRunner &game,
                             Unordered_State &current) {
    auto tigerMoves = game.validMoves(current, current.get_tiger());
    for(int i = 0; i < tigerMoves.second.second; i++){
        if(tigerMoves.second.first[i]){
            //std::cout << "Can capture if jumping too " << tigerMoves.first[i].row << ", " << tigerMoves.first[i].col << std::endl;
            scores[tigerMoves.first[i].row][tigerMoves.first[i].col] += 100;
        }else{
            //std::cout << "Safe Tiger move at " << tigerMoves.first[i].row << ", " << tigerMoves.first[i].col << std::endl;
        }
    }
    delete[] tigerMoves.first;
    delete[] tigerMoves.second.first;
}
