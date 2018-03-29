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

string const graph = "BOARD SQUARE SECTION DIMENSIONS\n9 9\nUNUSUAL EDGES\n2 0 4 1 3 1 5\n2 1 3 2 2\n1 2 2 3 1\n"
        "1 3 1 4 0\n1 4 0 5 1\n1 5 1 6 2\n1 6 2 7 3\n1 7 3 8 4\n1 1 5 2 6\n1 2 6 3 7\n1 3 7 4 8\n1 4 8 5 7\n"
        "1 5 7 6 6\n1 6 6 7 5\n1 7 5 8 4\n2 8 4 10 5 10 3\n2 10 5 12 6 12 4\n2 10 3 12 4 12 2\n"
        "1 12 2 14 3\n1 12 6 14 5\n2 12 4 14 5 14 3\n1 14 5 16 4\n1 14 3 16 4";
string const startPos = "TIGER POS\n4 12\nMEN POS\n0 0\n0 1\n0 2\n"
        "0 3\n0 4\n0 5\n0 6\n0 7\n0 8\n1 0\n1 1\n1 2\n"
        "1 3\n1 4\n1 5\n1 6\n1 7\n1 8";
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
    int square_section_columns,
        square_section_rows;
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

#endif //TIGER_GAME_GAME_RUNNER_H
