//
// Created by iancl on 3/25/2018.
//

#ifndef TIGER_GAME_GAME_RUNNER_H
#define TIGER_GAME_GAME_RUNNER_H

#include "function_api.h"
#include <iostream>
#include <map>
#include <list>
#include <sstream>

string const graph = "BOARD SQUARE SECTION DIMENSIONS\n17 9\nUNUSUAL EDGES"
                     "\n2 16 4 15 3 15 5\n2 15 3 14 2\n1 14 2 13 1\n"
        "1 13 1 12 0\n1 12 0 11 1\n1 11 1 10 2\n1 10 2 9 3\n1 9 3 8 4\n"
        "1 15 5 14 6\n1 14 6 13 7\n1 13 7 12 8\n1 12 8 11 7\n1 11 7 10 6\n"
        "1 10 6 9 5\n1 9 5 8 4\n2 8 4 6 5 6 3\n2 6 5 4 6 4 4\n2 6 3 4 4 4 2\n"
        "1 4 2 2 3\n1 4 6 2 5\n2 4 4 2 5 2 3\n1 2 5 0 4\n1 2 3 0 4";
string const startPos = "TIGER POS\n4 4\nMEN POS\n16 0\n16 1\n16 2\n"
        "16 3\n16 4\n16 5\n16 6\n16 7\n16 8\n17 0\n17 1\n17 2\n"
        "17 3\n17 4\n17 5\n17 6\n17 7\n17 8";
/**
 * This class runs a game. It can be used like Booth's API to fetch moves and run them on the Game
 *
 * However, It can also be used in our program to find moves of a piece because it knows the board layout.
 * author: Ian Laird
 */
class GameRunner{
    bool manJumpedLastCheck;
    int manJumpedCol, manJumpedRow;
    vector <Token_t> * gameState;
    int col_boundary,
        row_boundary,
        tiger_cage_row_offset;
    map<Point_t, list<Point_t> > * extendedGraph;
public:
    /**
     * Description: Sees is the indicated move is valid. i.e. Can the indicated
     *              token move to the position specified.
     *              return: true means valid move, false means invalid
     * precondition: game state is valid
     * postcondition: game is unchanged
     * return: First pair element indicates if a player won, second pair element
     *             indicates which player if a win occurred
     */
    bool isValidMove(vector <Token_t> const &, Move_t) ;
    /*
     * description: Runs the game until tie or victory for Tiger or Men
     * precondition: there is a validly named move function it can use
     * postcondition: One game is played to completion
     * return: First indicates if a player won, second incates
     *             which player if a win occurred
     */
    pair<bool, Color_t> playGame();

    //These functions can be used in our AI implementation
    /*
     * description: Custom Constructor
     *              Reads in game board and starting positions for pieces
     *              from files. This is the one that is used for testing reasons.
     * precondition: GameRunner does not exist
     * Return: none: GameRunner is created
     */
    GameRunner(std::istream & graphFile, std::istream & startingPos);
    /*
     * description: Default Constructor
     *              Reads in game board from string streams.
     *              This is the one that should be used in our AI
     * precondition: GameRunner does not exist
     * Return: none: GameRunner is created
     */
    GameRunner();
    /*
     * description: destructor
     * precondition: GameRunner exists
     * postcondition: GameRunner is destroyed and all memory freed.
     * Return: none
     */
    ~GameRunner(){
        delete this->extendedGraph;
        delete this->gameState;
    }
    /*
     * description: returns all Moves a Token can make.
     *                  will return Tiger jump moves.
     * precondition: Board state is valid
     * postcondition: Board is unchanged.
     * Return: a pointer to the Moves along with the # of moves.
     */
    pair<Point_t *, int> validMoves(vector <Token_t> const &, Token_t);
    /*
     * description: sees if a player has won (Tiger or Men)
     * precondition: Board state is valid
     * postcondition:  Board is unchanged
     * return: true is a player has won(player is stored in refernce variable)
     */
    bool evaluateWinState( vector <Token_t> &, Color_t & color);

};

/*
 * description: operator==
 * return: true is the two Move_t are equal
 */
bool operator==(Move_t a, Move_t b);
/*
 * description: operator==
 * return: true is the two Point_t are equal
 */
bool operator==(Point_t a, Point_t b);
/*
 * description: operator==
 * return: true is the two Token_t are equal
 */
bool operator==(Token_t a, Token_t b);

bool operator<(Point_t a, Point_t b);

#endif //TIGER_GAME_GAME_RUNNER_H
