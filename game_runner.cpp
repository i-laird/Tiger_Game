//
// Created by iancl on 3/25/2018.
//

#include "game_runner.h"

bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) {
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
        if(validMove){
            this->manJumpedLastCheck = true;
            this->mainJumpedCol = jumpedManCol;
            this->manJumpedRow = jumpedManRow;
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

/*
 * Deafault Constructor can be used to creact default start game
 */
GameRunner::GameRunner(){
    istringstream graphFile(graph), startingPos(startPos);
    this->gameState = new vector<Token_t>();
    this->extendedGraph = new map<Point_t, list<Point_t> >();
    int readNum;
    Token_t tempToken;
    Point_t tempPoint, tempPoint2;
    list<Point_t> tempList;
    map<Point_t, list<Point_t> >::iterator mapIter, mapIter2;
    //Read in the board
    graphFile.ignore(1000, '\n');
    //Read in the square section
    graphFile >> this->square_section_rows >> this->square_section_columns;
    //Read in the Unusual edges
    graphFile.ignore(1000, '\n');
    while(graphFile >> readNum){
        graphFile >> tempPoint.row >> tempPoint.col;
        mapIter = extendedGraph->find(tempPoint);
        if(mapIter == extendedGraph->end()){
            extendedGraph->insert(make_pair(tempPoint, tempList));
            mapIter = extendedGraph->find(tempPoint);
        }
        for(int i = 0; i < readNum; i++) {
            graphFile >> tempPoint2.row >> tempPoint2.col;
            mapIter->second.push_back(tempPoint);
            mapIter2 = extendedGraph->find(tempPoint2);
            if(mapIter2 == extendedGraph->end()){
                extendedGraph->insert(make_pair(tempPoint2, tempList));
                mapIter2 = extendedGraph->find(tempPoint2);
            }
            mapIter2->second.push_back(tempPoint);
        }
    }

    //Read in the piece locations
    tempToken.color = RED;
    startingPos.ignore(1000, '\n');
    startingPos >> tempToken.location.row >> tempToken.location.col;
    this->gameState->push_back(tempToken);
    tempToken.color = BLUE;
    //Read in the men
    while(startingPos >> tempToken.location.row >> tempToken.location.col){
        this->gameState->push_back(tempToken);
    }
}

/*
 * Custom constructor can make custom game from files
 */
GameRunner::GameRunner(std::istream & graphFile, std::istream & startingPos){
    this->gameState = new vector<Token_t>();
    this->extendedGraph = new map<Point_t, list<Point_t> >();
    int readNum;
    Token_t tempToken;
    Point_t tempPoint, tempPoint2;
    list<Point_t> tempList;
    map<Point_t, list<Point_t> >::iterator mapIter, mapIter2;
    //Read in the board
    graphFile.ignore(1000, '\n');
    //Read in the square section
    graphFile >> this->square_section_rows >> this->square_section_columns;
    //Read in the Unusual edges
    graphFile.ignore(1000, '\n');
    while(graphFile >> readNum){
        graphFile >> tempPoint.row >> tempPoint.col;
        mapIter = extendedGraph->find(tempPoint);
        if(mapIter == extendedGraph->end()){
            extendedGraph->insert(make_pair(tempPoint, tempList));
            mapIter = extendedGraph->find(tempPoint);
        }
        for(int i = 0; i < readNum; i++) {
            graphFile >> tempPoint2.row >> tempPoint2.col;
            mapIter->second.push_back(tempPoint);
            mapIter2 = extendedGraph->find(tempPoint2);
            if(mapIter2 == extendedGraph->end()){
                extendedGraph->insert(make_pair(tempPoint2, tempList));
                mapIter2 = extendedGraph->find(tempPoint2);
            }
            mapIter2->second.push_back(tempPoint);
        }
    }

    //Read in the piece locations
    tempToken.color = RED;
    startingPos.ignore(1000, '\n');
    startingPos >> tempToken.location.row >> tempToken.location.col;
    this->gameState->push_back(tempToken);
    tempToken.color = BLUE;
    //Read in the men
    while(startingPos >> tempToken.location.row >> tempToken.location.col){
        this->gameState->push_back(tempToken);
    }

}

pair<bool, Color_t> GameRunner::playGame(){
    Color_t turn = RED, winner;
    Move_t returnedMove;
    int count = 0;
    //Keep playing until game is finished or 10000 turns have passed
    while(count < 10000 && !this->evaluateWinState(*this->gameState, winner)){
        this->manJumpedLastCheck = false;
        count++;
        returnedMove = My_Move(*this->gameState, turn);
        if(isValidMove(*this->gameState, returnedMove)){
            for(vector<Token_t>::iterator vectorIterator = gameState->begin();
                vectorIterator != gameState->end(); vectorIterator++) {
                    //See if the encountered spot is the moved token
                    if ((*vectorIterator).location == returnedMove.token.location) {
                        (*vectorIterator).location = returnedMove.destination;
                    }
                    //See if a man was killed that turn
                    if (turn == RED && this->manJumpedLastCheck) {
                        if ((*vectorIterator).location.row == manJumpedRow &&
                            (*vectorIterator).location.col == manJumpedCol) {
                            this->gameState->erase(vectorIterator);
                            break;
                        }
                    }
            }
        }
        else{
            //Move randomly
        }
        turn = ~turn;
    }
    if(count >= 10000) {
        return make_pair(false, RED);
    }else if(winner == RED){
        return make_pair(true, RED);
    }
    return make_pair(true, BLUE);
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


