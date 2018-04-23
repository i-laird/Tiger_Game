//
// Created by iancl on 3/25/2018.
//

#include <assert.h>
#include <set>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "game_runner.h"
Move_t  My_Move(vector<Token_t>, Color_t turn){

}

/*
 * Default Constructor can be used to create default start game
 */
GameRunner::GameRunner(){
    istringstream graphFile(graph), startingPos(startPos);
    this->createGraph(graphFile, startingPos);
}

/*
 * Custom constructor can make custom game with new map and new starting pos
 */
GameRunner::GameRunner(std::istream & graphFile, std::istream & startingPos){
    this->createGraph(graphFile, startingPos);
}

/*
 * Custom constructor can make custom game with default map but custom locations
 */
GameRunner::GameRunner(std::istream & startingPos){
    istringstream graphFile(graph);
    this->createGraph(graphFile, startingPos);
}

void GameRunner::createGraph(std::istream & graphFile, std::istream & startingPos){
    this->tiger_cage_row_offset = 4;
    string trashline = "";
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
    getline(startingPos, trashline);
    startingPos >> tempToken.location.row >> tempToken.location.col;
    this->gameState->push_back(tempToken);
    tempToken.color = BLUE;
    startingPos.ignore();
    getline(startingPos, trashline);
    //Read in the men
    while(startingPos >> tempToken.location.row >> tempToken.location.col){
        this->gameState->push_back(tempToken);
    }
}

bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) {
    this->manJumpedLastCheck = false;
    Point_t jumpedMan;
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
        if(moves[i] == move.token && !validMove) {
            map<Point_t, list<Point_t> >::iterator mapIter; // to check for special edges

            bool inSquareSection = false;
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
                if((rowDifference > 1 || colDifference > 1)) {
                    return false;
                }
            }
            if(move.token.color == RED){
                if((rowDifference > 2 || colDifference > 2)) {
                    return false;
                }
                //See if tiger jumped man in square section
                if(rowDifference == 2 || colDifference == 2){
                    tigerJumpedMan = true;
                    //Now find coordinates of jumped dude
                    jumpedMan.col = jumpedManCol = (move.destination.col + origCol) / 2;
                    jumpedMan.row = jumpedManRow = (move.destination.row + origRow) / 2;
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


bool GameRunner::isValidMove(Unordered_State const & st, Move_t move) {
    this->manJumpedLastCheck = false;
    Point_t jumpedMan;
    Point_t from = move.token.location, to = move.destination;
    // if easily seen to be out of bounds, return false
    if(to.col < 0 || to.row < 0 ||
       to.col >= col_boundary || to.row >= row_boundary) {
            return false;
   }

    bool validMove = false, tigerJumpedMan = false;
    // if the destination is occupied, return false
    if(st.is_occupied(to)) {
        return false;
    }
    //Checking if move indicated is valid
    map<Point_t, list<Point_t> >::iterator mapIter; // to check for special edges
    bool inSquareSection = false;
    Point_t diff = abs(to - from);
    if(from.row >= tiger_cage_row_offset && from.row < row_boundary && from.col < col_boundary
       && to.row >= tiger_cage_row_offset && to.row < row_boundary && to.col < col_boundary){
            inSquareSection = true;
    }
    //Men can only move 1
    if(move.token.color == BLUE){
        if((diff.row > 1 || diff.col > 1)) {
            return false;
        }
    }
    if(move.token.color == RED){
        if(diff.row > 2 || diff.col > 2) {
            return false;
        }
        //See if tiger jumped man in square section
        if(diff.row == 2 || diff.col == 2){
            tigerJumpedMan = true;
            //Now find coordinates of jumped dude
            jumpedMan = (to + from) / 2;
        }
    }
    //See if the move starts and ends in the Square section and is not diagonal
    if((diff.row > 0 && diff.col == 0) || (diff.col > 0 && diff.row == 0)){
        if(inSquareSection){
            validMove = true;
        }
    }
    //See if the move involved an unusual edge in some way
    else if((mapIter = extendedGraph->find(from)) != extendedGraph->end()){
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
                    if(to == *listIter2){
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
    }//Move start position invalid
    else{
        return false;
    }

    //See if a man was actual present where the tiger is said to have jumped him
    if(tigerJumpedMan && validMove){
        validMove = false;
        //See if a man is present at the jumped position
        if(st.is_occupied(jumpedMan)) {
            validMove = true;
        }
        //If the Tiger jump was actually valid store the info for later use
        if(validMove){
            this->manJumpedLastCheck = true;
            this->manJumpedCol = jumpedMan.col;
            this->manJumpedRow = jumpedMan.row;
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
                for(unsigned int i = 1; i < gameState->size(); i++){
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
    Point_t * validPoints = new Point_t[MAX_NUMBER_MOVES];
    bool * jumpMade = new bool[MAX_NUMBER_MOVES];
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


pair<Point_t *, pair<bool *, int> > GameRunner::validMoves(Unordered_State const & boardState, Token_t piece){
    //Maximum number of valid moves
    Point_t * validPoints = new Point_t[MAX_NUMBER_MOVES];
    bool * jumpMade = new bool[MAX_NUMBER_MOVES];
    int size = 0;
    Move_t tempMove, jumpMove;
    //This accounts for basic non diagonal moves
    for(int i = 0; i < 4; i++){
        Point_t dir;
        switch(i) {
            case 0:
                dir = UP;
                break;
            case 1:
                dir = DOWN;
                break;
            case 2:
                dir = RIGHT;
                break;
            case 3:
                dir = LEFT;
        }
        tempMove = make_move_in_direction(piece, dir);
        jumpMove = make_move_in_direction(piece, 2 * dir);
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
    tempMove.token = jumpMove.token = piece; // ensure token is piece
    map<Point_t, list<Point_t> >::const_iterator mapIter = extendedGraph->find(piece.location);
    list<Point_t>::const_iterator listIter;
    if(mapIter != extendedGraph->end()){
        listIter = mapIter->second.begin();
        while(listIter != mapIter->second.end()){
            tempMove.destination = *listIter;
            jumpMove.destination = tempMove.destination +
                                   (tempMove.destination - tempMove.token.location);
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


Move_t GameRunner::Tiger_Move(vector<Token_t> & tokens, int randomProbability){
    Move_t returnMove;
    returnMove.token.location = tokens[0].location;
    returnMove.token.color = RED;

    srand(time(NULL));
    //Get all of the valid moves for the Tiger
    pair<Point_t *, pair<bool *, int> > returnMoves = this->validMoves(tokens, tokens[0]);
    //Act randomly
    if(9 -(rand() % 10) < randomProbability){
        //If random make random move
        returnMove.destination = returnMoves.first[rand() % returnMoves.second.second];
        delete [] returnMoves.second.first;
        delete [] returnMoves.first;
        return returnMove;
    }
    set<Point_t> checkPoints;
    pair<Point_t * , pair< bool *, int > > tokenMoves;
    static Point_t previousLocation;
    Point_t closestPoint, record = NULL_POINT, record2;
    int smallestRowColDistance = 1000, rowDifference, colDifference, destRow,destCol, origRow, origCol, totaler;

    bool moveFound = false;
    origCol = tokens[0].location.row;
    origRow = tokens[0].location.col;

    //First see if jump can be made
    //Will probably implement min max here later
    for(int i = 0; i < returnMoves.second.second; i++){
        //Greedily selects a jump
        if(returnMoves.second.first[i]){
            returnMove.destination = returnMoves.first[i];
            moveFound = true;
        }
    }
    if(!moveFound){
        //Temporarily reset tiger Position
        record2 = tokens[0].location;
        tokens[0].location = record;
        //Add all points within 1 of the Men to the set to be evaluated
        for(unsigned int i = 1; i < tokens.size(); i++){
            tokenMoves = validMoves(tokens, tokens[i]);
            for(int j = 0; j < tokenMoves.second.second;j++){
                checkPoints.insert(tokenMoves.first[j]);
            }
            delete [] tokenMoves.second.first;
            delete [] tokenMoves.first;
        }
        tokens[0].location = record2;
        //Now find the closest point
        for(Point_t toCheck : checkPoints){
            destRow = toCheck.row;
            destCol = toCheck.col;
            colDifference = destCol - origCol;
            rowDifference = destRow - origRow;
            colDifference = (colDifference < 0) ? colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 : rowDifference;
            totaler = rowDifference + colDifference;
            //see if it is a point that is more desirable i.e. has diagonal
            //We want tiger to gravitate to diagonal edges
            if(this->extendedGraph->find(toCheck) != extendedGraph->end()){
                totaler -= 3;
            }
            if(totaler < smallestRowColDistance){
                smallestRowColDistance = totaler;
                closestPoint = toCheck;
            }
        }
        //See if tiger is already on the closest point
        bool alreadyClosest = (closestPoint == tokens[0].location);
        //See if should alternate if already there
        if(alreadyClosest) {
            map<Point_t, list<Point_t>>::const_iterator mapIter = this->extendedGraph->find(tokens[0].location);
            //See if non traditional move can be made because they are harder to counter
            if (mapIter != extendedGraph->end()) {
                for (Point_t temp : mapIter->second) {
                    returnMove.destination = temp;
                    if (isValidMove(tokens, returnMove)) {
                        moveFound = true;
                        break;
                    }
                }
            }
            //If no non traditional move can be found see if can just move
            // back to previous location
            if (!moveFound) {
                //Try and go upwards
                for(int k = 0; !moveFound && k < returnMoves.second.second; k++) {
                    returnMove.destination = returnMoves.first[k];
                    if((returnMove.destination.row - origRow != 0)) {
                        moveFound = true;
                    }
                }
                if(!moveFound){
                    moveFound = true;
                    record = returnMove.destination;
                    returnMove.destination = previousLocation;
                    if (!isValidMove(tokens, returnMove))
                        returnMove.destination = record;
                }
            }
        }
        //Case not already as close as can be then I will BFS to him
        else{
            bool success;
            returnMove.destination = BFS_To_Point(tokens, 0, closestPoint, RED, success);
            //If no path can be found then just make a move
            if(!success){
                returnMove.destination = returnMoves.first[0];
            }
        }
    }
    delete [] returnMoves.second.first;
    delete [] returnMoves.first;
    previousLocation = tokens[0].location;
    return  returnMove;
}

Point_t GameRunner::BFS_To_Point(vector<Token_t> mapLayout, int tokenIndex, Point_t desiredLoc, Color_t color, bool & success){
    queue<Point_t> frontier;
    map<Point_t, Point_t> previous;
    Point_t evaluatePoint, originalPoint = mapLayout[tokenIndex].location, temp;
    Token_t currToken;
    currToken.color = color;
    frontier.push(mapLayout[tokenIndex].location);
    previous[currToken.location] = currToken.location;
    pair<Point_t * , pair< bool *, int > > tokenMoves;
    while(!frontier.empty()){
        //Reseting the position because validMoves will check if a piece actually exists there!
        //Okay to change layout because vector is a copy :-)
        currToken.location = mapLayout[tokenIndex].location = frontier.front();
        frontier.pop();
        tokenMoves = this->validMoves(mapLayout, currToken);
        //See if done
        if(currToken.location == desiredLoc){
            break;
        }
        for(int i = 0; i < tokenMoves.second.second; i++){
            evaluatePoint = tokenMoves.first[i];
            //See if the key has a predecessor i.e. exists if not add
            if(previous.insert(make_pair(evaluatePoint, currToken.location)).second) {
                frontier.push(evaluatePoint);
            }
        }
        delete [] tokenMoves.second.first;
        delete [] tokenMoves.first;
    }
    if(currToken.location == desiredLoc) {
        //Now find what Point should be moved to
        temp = evaluatePoint = currToken.location;;
        while(!((temp = previous[temp]) == originalPoint)){
            evaluatePoint = temp;
        }
        success = true;
    }
    else{
        success = false;
    }
    return evaluatePoint;

}

int GameRunner::getRandomizer(vector<Token_t> & tokens){
    //First find the average row
    int rowTotal = 0, rowAverage, returnVal;
    for(Token_t & temp : tokens){
        rowTotal += temp.location.row;
    }
    rowAverage = rowTotal / tokens.size();
    //Now just return a higher randomizer based on stage of play
    if(rowAverage <= 4)
        return 2;
    if(rowAverage <= 10)
        return 1;
    return 0;
}

Move_t GameRunner::Undeterministic_Tiger_Move(vector<Token_t> & pieces){
    static list<Point_t> previousPoints;
    set<Point_t> movedPoints(previousPoints.begin(), previousPoints.end());
    Move_t returnMove;
    returnMove = (movedPoints.size() == 2 ? this->Tiger_Move(pieces, 0) :
                  this->Tiger_Move(pieces, this->getRandomizer(pieces)));
    previousPoints.push_back(returnMove.destination);
    if(previousPoints.size() > 6)
        previousPoints.pop_front();
    return returnMove;
}



