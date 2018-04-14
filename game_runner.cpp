//
// Created by iancl on 3/25/2018.
//

#include <assert.h>
#include <set>
#include "game_runner.h"
Move_t  My_Move(vector<Token_t>, Color_t turn){

}

bool operator<(Point_t a, Point_t b){
    return (a.row < b.row ? true : (a.col < b.col && a.row == b.col));
}
bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) {
    this->manJumpedLastCheck = false;
    if(move.destination.col < 0 || move.destination.row < 0
       || move.destination.col >= col_boundary || move.destination.row >= row_boundary)
            return false;

    bool validMove = false, tigerJumpedMan = false;
    int jumpedManCol, jumpedManRow;
    for(unsigned i = 0; i < moves.size(); i++){
        //See if another token occupies that location
        if(moves[i].location == move.destination){
            return false;
        }
        //Checking if move indicated is valid IF token found is right token
        if(moves[i] == move.token && validMove == false) {
            bool inSquareSection = false;
            map<Point_t, list<Point_t> >::iterator mapIter;
            int destRow = move.destination.row, destCol = move.destination.col,
                origRow = moves[i].location.row, origCol = moves[i].location.col;
            int colDifference = destCol - origCol,
                rowDifference = destRow - origRow;
            colDifference = (colDifference < 0) ? colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 : rowDifference;
            if(origRow >= tiger_cage_row_offset && origRow < row_boundary && origCol < col_boundary
               && destRow >= tiger_cage_row_offset && destRow < row_boundary && destCol < col_boundary){
                    inSquareSection = true;
            }
            //Men can only move 1 ever except Tiger cage
            if(move.token.color == BLUE){
                //See if move starts and ends in Square section but has move diff > 1
                if(inSquareSection){
                    if((rowDifference > 1 || colDifference > 1)) {
                        return false;
                    }
                }
                //If in Tiger cage can move 2 max
                else{
                    if(rowDifference > 2 || colDifference > 2){
                        return false;
                    }
                }
            }
            if(move.token.color == RED){
                //See if tiger move started and ended in square section of board
                if(inSquareSection){
                    //Even w/ jump cannot move more than 2 in rows or columns
                    if((rowDifference > 2 || colDifference > 2)) {
                        return false;
                    }
                    //See if tiger jumped man in square section
                    if(rowDifference == 2 || colDifference == 2){
                        tigerJumpedMan = true;
                    }
                }
                else{
                    //Tiger can NEVER move more than 4 in row or colum even w/ tiger cage jump
                    if(rowDifference > 4 || colDifference > 4){
                        return false;
                    }
                    //See if tiger jumped man in Tiger cage
                    if(rowDifference > 2 || colDifference > 2){
                        tigerJumpedMan = true;
                    }
                }
            }
            //See if the move starts and ends in the Square section and is not diagonal
            if((rowDifference > 0 && colDifference == 0) || (colDifference > 0 && rowDifference == 0)){
                if(inSquareSection){
                    validMove = true;
                }
            }
            //See if the move involved an unusual edge in some way
            else if((mapIter = extendedGraph->find(moves[i].location)) != extendedGraph->end()){
                Point_t jumpedMan;
                if(tigerJumpedMan) {
                    //Now find coordinates of jumped dude
                    jumpedMan.col = jumpedManCol = (move.destination.col + origCol) / 2;
                    jumpedMan.row = jumpedManRow = (move.destination.row + origRow) / 2;
                }
                list<Point_t>::const_iterator listIter = mapIter->second.begin();
                while(!validMove && listIter != mapIter->second.end()){
                    if(move.destination == *listIter){
                        validMove = true;
                    }
                    else if(tigerJumpedMan && jumpedMan == *listIter){
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
                //Move end position not reachable from the indicated start position
                if(!validMove){
                    return false;
                }
            }
            //Move start position invalid
            else{
                return false;
            }
        }
    }
    //See if a man was actual present where the tiger is said to have jumped him
    if(tigerJumpedMan && validMove){
        validMove = false;
        //See if a man is present at the jumped position
        for(unsigned i = 1 ; i < moves.size() && !validMove; i++){
            if(moves[i].location.row == jumpedManRow && moves[i].location.col == jumpedManCol){
                validMove = true;
            }
        }
        //If the Tiger jump was actually valid store the info for later use
        if(validMove){
            this->manJumpedLastCheck = true;
            this->manJumpedCol = jumpedManCol;
            this->manJumpedRow = jumpedManRow;
        }
    }
    return validMove;
}

bool GameRunner::evaluateWinState( vector <Token_t> & tokens, Color_t & color){
    bool returnFlag = false;
    pair<Point_t *, pair<bool *, int> > moveReceiver = this->validMoves(tokens, tokens[0]);
    //Means the tiger cannot move so MEN WIN
    if(moveReceiver.second.second == 0){
        color = BLUE;
        returnFlag = true;
    }
    //If at least one man alive Tiger has not won yet
    if(!returnFlag && tokens.size() > 1){
        color = RED;
        returnFlag = true;
    }
    delete [] moveReceiver.second.first;
    delete [] moveReceiver.first;
    return returnFlag;
}

/*
 * Default Constructor can be used to create default start game
 */
GameRunner::GameRunner(){
    this->tiger_cage_row_offset = 4;
    istringstream graphFile(graph), startingPos(startPos);
    string trashline;
    this->gameState = new vector<Token_t>();
    this->extendedGraph = new map<Point_t, list<Point_t> >();
    int readNum;
    Token_t tempToken;
    Point_t tempPoint, tempPoint2;
    list<Point_t> tempList;
    map<Point_t, list<Point_t> >::iterator mapIter, mapIter2;
    //Read in the board
    getline(graphFile, trashline);
    //Read in the square section
    graphFile >> this->row_boundary >> this->col_boundary;
    //Read in the Unusual edges
    graphFile.ignore();
    getline(graphFile, trashline);
    while(graphFile >> readNum){
        graphFile >> tempPoint.row >> tempPoint.col;
        mapIter = (extendedGraph->insert(make_pair(tempPoint, tempList))).first;
        for(int i = 0; i < readNum; i++) {
            graphFile >> tempPoint2.row >> tempPoint2.col;
            mapIter->second.push_back(tempPoint2);
            mapIter2 = (extendedGraph->insert(make_pair(tempPoint2, tempList))).first;
            mapIter2->second.push_back(tempPoint);
        }
    }

    //Read in the piece locations
    tempToken.color = RED;
    getline(graphFile, trashline);
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
    this->tiger_cage_row_offset = 4;
    string trashline;
    this->gameState = new vector<Token_t>();
    this->extendedGraph = new map<Point_t, list<Point_t> >();
    int readNum;
    Token_t tempToken;
    Point_t tempPoint, tempPoint2;
    list<Point_t> tempList;
    map<Point_t, list<Point_t> >::iterator mapIter, mapIter2;
    //Read in the board
    getline(graphFile, trashline);
    //Read in the square section
    graphFile >> this->row_boundary >> this->col_boundary;
    //Read in the Unusual edges
    graphFile.ignore();
    getline(graphFile, trashline);
    while(graphFile >> readNum){
        graphFile >> tempPoint.row >> tempPoint.col;
        mapIter = (extendedGraph->insert(make_pair(tempPoint, tempList))).first;
        for(int i = 0; i < readNum; i++) {
            graphFile >> tempPoint2.row >> tempPoint2.col;
            mapIter->second.push_back(tempPoint2);
            mapIter2 = (extendedGraph->insert(make_pair(tempPoint2, tempList))).first;
            mapIter2->second.push_back(tempPoint);
        }
    }

    //Read in the piece locations
    tempToken.color = RED;
    getline(graphFile, trashline);
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
    int counter = 0;
    //Keep playing until game is finished or 10000 turns have passed
    while(counter < 10000 && !this->evaluateWinState(*this->gameState, winner)){
        this->manJumpedLastCheck = false;
        counter++;
        returnedMove = My_Move(*this->gameState, turn);
        //If not valid move I will perform first available move
        if(!isValidMove(*this->gameState, returnedMove)){
            if(turn == RED){
                returnedMove.token = (*gameState)[0];
                returnedMove.destination = this->validMoves(*gameState, returnedMove.token).first[0];
            }
            else{
                for(int i = 1; i < gameState->size(); i++){
                    returnedMove.token = (*gameState)[i];
                    pair<Point_t *, pair<bool *, int > > returnPair = this->validMoves(*gameState, returnedMove.token);
                    if(returnPair.second.second > 0){
                        returnedMove.destination = returnPair.first[0];
                        break;
                    }
                }
            }
        }
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
        turn = (turn == RED ? RED : BLUE);
    }
    if(counter >= 10000) {
        return make_pair(false, RED);
    }else if(winner == RED){
        return make_pair(true, RED);
    }
    return make_pair(true, BLUE);
}

pair<Point_t *, pair<bool *, int> > GameRunner::validMoves(vector <Token_t> const & boardState, Token_t piece){
    //Maximum number of valid moves
    Point_t * validPoints = new Point_t[6];
    bool * jumpMade = new bool[6];
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
        //See if moving simply UP,DOWN,LEft,RiGHT 1 is valid
        if(this->isValidMove(boardState, tempMove)){
            validPoints[size] = tempMove.destination;
            jumpMade[size] = false;
            size += 1;
        } //See if moving 2 UP,DOWN,LEFT,RIGHt works
        else if (piece.color == RED && isValidMove(boardState, jumpMove)){
            validPoints[size] = jumpMove.destination;
            jumpMade[size] = true;
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
            //See if diagonal move valid
            if(isValidMove(boardState, tempMove)) {
                validPoints[size] = tempMove.destination;
                jumpMade[size] = false;
                size += 1;
            }//See if man can be jumped with diagonal
            else if(piece.color == RED && isValidMove(boardState, jumpMove)){
                validPoints[size] = jumpMove.destination;
                jumpMade[size] = true;
                size += 1;
            }
            listIter++;
        }
    }
    //return the pointer coupled with the number of elements stored in it
    return make_pair(validPoints, make_pair(jumpMade, size));
}

Move_t GameRunner::Tiger_Move(vector<Token_t> & tokens){
    Move_t returnMove;
    returnMove.token.location = tokens[0].location;
    returnMove.token.color = RED;
    static Point_t previousLocation;
    Point_t closestPoint;
    int smallestRowColDistance = 1000, rowDifference, colDifference, destRow,destCol, origRow, origCol;
    //Get all of the valid moves for the Tiger
    pair<Point_t *, pair<bool *, int> > returnMoves = this->validMoves(tokens, tokens[0]);
    bool moveFound = false;
    origCol = tokens[0].location.row;
    origRow=tokens[0].location.col;

    //First see if jumo can be made
    //Will probably implement min max here later
    for(int i = 0; i < returnMoves.second.second; i++){
        if(returnMoves.second.first[i]){
            returnMove.destination = returnMoves.first[i];
            moveFound = true;
        }
    }
    if(!moveFound){
        //Now find the closest point
        for(int i = 1; i < tokens.size();i++){
            destRow = tokens[i].location.row;
            destCol=tokens[i].location.col;
            colDifference = destCol - origCol;
            rowDifference = destRow - origRow;
            colDifference = (colDifference < 0) ? colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 : rowDifference;
            if(colDifference + rowDifference < smallestRowColDistance){
                smallestRowColDistance = colDifference + rowDifference;
            }
            closestPoint = tokens[i].location;
        }
        //See if closest piece is within 1 move
        bool within1Move = false;
        for(int i =0 ; i < returnMoves.second.second;i++){
            if(returnMoves.first[i] == closestPoint){
                within1Move = true;
            }
        }
        //See if should alternate
        if(within1Move) {
            map<Point_t, list<Point_t>>::const_iterator mapIter = this->extendedGraph->find(tokens[0].location);
            if (mapIter != extendedGraph->end()) {
                for (Point_t temp : mapIter->second) {
                    returnMove.destination = temp;
                    if (isValidMove(tokens, returnMove)) {
                        moveFound = true;
                        break;
                    }
                }
            }
            //If alternating does not work
            if (!moveFound) {
                returnMove.destination = previousLocation;
                if (isValidMove(tokens, returnMove)) {
                    moveFound = true;
                } else {
                    //If no special move or previous just make whatever move
                    returnMove.destination = returnMoves.first[0];
                    moveFound = true;
                }
            }
        }
        else{
            //TODO IMPLEMENT THIS FUNCTION
            //returnMove.destination = BFS_To_Point(closestPoint);
        }
    }
    delete [] returnMoves.second.first;
    delete [] returnMoves.first;
    previousLocation = tokens[0].location;
    return  returnMove;
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


