//
// Created by Aidan on 4/18/2018.
//

#include "Scored_MMover.h"

int ** Scored_MMover::genScores(const State &s) {
    int** out = new int*[11];
    for(int i = 0; i < 13; i++){
        out[i] = new int[9];
        for(int j = 0; j < 9; j++){
            out[i][j] = 0;
        }
    }
    return out;
}

Move_t Scored_MMover::next_move(State s) {
    Move_t best = {s[0], {0, 0}};
    int bestScore = 0;
    bool first = true;
    int** scores = genScores(s);
    for(Token_t token : s){
        if(token.color == BLUE){
            for(Move_t move : brd.listPossible(s, token)){
                int delta = scoreDelta(scores, move);
                if(first || delta > bestScore){
                    first = false;
                    best = move;
                    bestScore = delta;
                }
            }
        }
    }
    return best;
}

int Scored_MMover::scoreDelta(int ** scores, Move_t move) {
    return scores[move.destination.row][move.destination.col] - scores[move.token.location.row][move.token.location.col];
}
