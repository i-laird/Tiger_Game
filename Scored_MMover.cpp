//
// Created by Aidan on 4/18/2018.
//

#include "Scored_MMover.h"

int ** Scored_MMover::genScores() {
    int** out = new int*[11];
    for(int i = 0; i < 13; i++){
        out[i] = new int[9];
        for(int j = 0; j < 9; j++){
            out[i][j] = 0;
        }
    }
    return out;
}

Move_t Scored_MMover::execute_move() {
    Move_t best = {static_cast<State>(current)[0], {0, 0}};
    int bestScore = 0;
    bool first = true;
    int** scores = genScores();
    for(Token_t token : static_cast<State>(current)){
        if(token.color == BLUE){
            auto moves = game.validMoves(current, token);
            for(int i = 0; i < moves.second.second; i++){
                if(!moves.second.first[i]){
                    Move_t move = {token, moves.first[i]};
                    int delta = scoreDelta(scores, move);
                    if(first || delta > bestScore){
                        first = false;
                        best = move;
                        bestScore = delta;
                    }
                }
            }
        }
    }
    return best;
}

int Scored_MMover::scoreDelta(int ** scores, Move_t move) {
    return scores[move.destination.row][move.destination.col] - scores[move.token.location.row][move.token.location.col];
}

Scored_MMover::Scored_MMover(const State &s) : Men_Mover(s) {}
