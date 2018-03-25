//
// Created by iancl on 3/25/2018.
//

#include "game_runner.h"

bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) const{
    for(unsigned i = 0; i < moves.size(); i++){
        if(moves[i] == move.token) {
            bool validMove = true;
            map<Point_t, list<Point_t> >::iterator mapIter;
            int destRow = move.destination.row, destCol = move.destination.col,
                origRow = moves[i].location.row, origCol = moves[i].location.col;
            int colDifference = move.destination.col - moves[i].location.col,
                rowDifference = move.destination.row - moves[i].location.row;
            colDifference = (colDifference < 0) ? colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 : rowDifference;
            if(rowDifference > 1 || colDifference > 1){
                return false;
            }
            //See if the move starts and ends in the Square section and is not diagonal
            if((rowDifference == 1 && colDifference == 0) || (colDifference == 1 && rowDifference == 0)){
                if(origRow <= this->square_section_rows && origRow >= 0 &&
                   origCol <= this->square_section_columns && origCol >= 0 &&
                   destRow <= this->square_section_rows && destRow >= 0 &&
                   destCol <= this->square_section_columns && destCol >= 0){
                        return true;
                }
            }
            else if((mapIter = extendedGraph->find(moves[i].location)) != extendedGraph->end()){
                list<Point_t>::const_iterator listIter = mapIter->second.begin();
                while(listIter != mapIter->second.end()){
                    if(move.destination == *listIter){
                        return true;
                    }
                }
            }
            return false;
        }
    }
}

bool GameRunner::evaluateWinState( vector <Token_t> & tokens, Color_t & color){
    pair<Point_t *, int> moveReceiver = this->validMoves(tokens, tokens[1]);
    //Means the tiger cannot move
    if(moveReceiver.second == 0){
        color = RED;
        return true;
    }

    //See if any man can move
    for(unsigned int i = 0; i < tokens.size(); i++){
        moveReceiver = this->validMoves(tokens, tokens[1]);
        if(moveReceiver.second == 0){
            color = BLUE;
            return true;
        }
    }

}



GameRunner::GameRunner(std::istream & graphFile, std::istream & startingPos){

}

Color_t GameRunner::playGame(){

}

pair<Point_t *, int> GameRunner::validMoves(vector <Token_t> const &, Token_t){

}

bool operator==(Move_t a, Move_t b){
    return a.token == b.token && a.destination == b.destination;
}
bool operator==(Point_t a, Point_t b){
    return a.col == b.col && b.row == a.row;
}
bool operator==(Token_t a, Token_t b){
    return a.color == b.color && a.location == b.location;
}


