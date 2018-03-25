//
// Created by iancl on 3/25/2018.
//

#include "game_runner.h"

bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) const{
    bool validMove = false, tigerJumpedMan = false;
    int jumpedManCol, jumpedManRow;
    for(unsigned i = 0; i < moves.size(); i++){
        //See if another token occupies that location
        if(moves[i].location == move.destination){
            return false;
        }
        //Now if it is a valid move at all
        if(moves[i] == move.token && validMove == false) {
            map<Point_t, list<Point_t> >::iterator mapIter;
            int destRow = move.destination.row, destCol = move.destination.col,
                origRow = moves[i].location.row, origCol = moves[i].location.col;
            int colDifference = move.destination.col - moves[i].location.col,
                rowDifference = move.destination.row - moves[i].location.row;
            colDifference = (colDifference < 0) ? colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 : rowDifference;
            if((move.token.color == BLUE && rowDifference > 1 || colDifference > 1) ||
                move.token.color == RED && rowDifference > 2 || colDifference > 2 ){
                    return false;
            }
            if(rowDifference == 2 || colDifference == 2){
                tigerJumpedMan = true;
            }
            //See if the move starts and ends in the Square section and is not diagonal
            if((rowDifference > 0 && colDifference == 0) || (colDifference > 1 && rowDifference == 0)){
                if(origRow <= this->square_section_rows && origRow >= 0 &&
                   origCol <= this->square_section_columns && origCol >= 0 &&
                   destRow <= this->square_section_rows && destRow >= 0 &&
                   destCol <= this->square_section_columns && destCol >= 0){
                        validMove = true;
                }
            }
            else if((mapIter = extendedGraph->find(moves[i].location)) != extendedGraph->end()){
                Point_t jumpedMan;
                if(tigerJumpedMan) {
                    //Now find coordinates of jumped dude
                    jumpedMan.col = jumpedManCol = (move.destination.col + origCol) / 2;
                    jumpedMan.row = jumpedManRow = (move.destination.row + origRow) / 2;
                }
                list<Point_t>::const_iterator listIter = mapIter->second.begin();
                while(listIter != mapIter->second.end()){
                    if(move.destination == *listIter){
                        validMove = true;
                    }
                    if(tigerJumpedMan && jumpedMan == *listIter){
                        map<Point_t, list<Point_t> >::iterator mapIter2;
                        mapIter2 = extendedGraph->find(jumpedMan);
                        list<Point_t>::const_iterator listIter2 = mapIter2->second.begin();
                        while(!validMove && listIter2 != mapIter2->second.end()){
                            if(move.destination == *listIter2){
                                validMove = true;
                            }
                            listIter2++;
                        }
                    }
                    listIter++;
                }
            }
            else{
                return false;
            }
        }
    }
    //See if a man was actual present where the tiger is said to have jumped him
    if(tigerJumpedMan){
        for(unsigned i = 1 ; i < moves.size() && validMove; i++){
            if(moves[i].location.row == jumpedManRow && moves[i].location.col == jumpedManCol){
                validMove = false;
            }
        }
    }
    return validMove;
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

pair<Point_t *, int> GameRunner::validMoves(vector <Token_t> const & boardState, Token_t piece){
    //Maximum number of valid moves
    Point_t * validPoints = new Point_t[6];
    int size = 0;
    Move_t tempMove, jumpMove;
    tempMove.token = piece;
    jumpMove.token = piece;
    //This accounts for basic non diagonal moves
    for(int i = 0; i < 4; i++){
        tempMove.destination = jumpMove.destination = piece.location;
        switch(i) {
            case 0:
                tempMove.destination.row += 1;
                jumpMove.destination.row += 2;
                break;
            case 1:
                tempMove.destination.row -= 1;
                jumpMove.destination.row -= 2;
                break;
            case 2:
                tempMove.destination.col += 1;
                jumpMove.destination.col += 2;
                break;
            case 3:
                tempMove.destination.col -= 1;
                jumpMove.destination.col -= 2;
        }
        if(this->isValidMove(boardState, tempMove) ||
           (piece.color == RED && isValidMove(boardState, jumpMove))){
                validPoints[size] = tempMove.destination;
                size += 1;
        }
    }
    //Now check for diagonal moves
    map<Point_t, list<Point_t> >::const_iterator mapIter = extendedGraph->find(piece.location);
    list<Point_t>::const_iterator listIter;
    if(mapIter != extendedGraph->end()){
        listIter = mapIter->second.begin();
        while(listIter != mapIter->second.end()){
            tempMove.destination = *listIter;
            jumpMove.destination.row = ((tempMove.destination.row - piece.location.row) * 2) + piece.location.row;
            jumpMove.destination.col = ((tempMove.destination.col - piece.location.col) * 2) + piece.location.col;
            if(isValidMove(boardState, tempMove) || (piece.color == RED && isValidMove(boardState, jumpMove)){
                validPoints[size] = tempMove.destination;
                size += 1;
            }
        }
    }
    return make_pair(validPoints, size);
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


