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
string const graph = "BOARD SQUARE SECTION DIMENSIONS\n13 9\nUNUSUAL EDGES"
                     "\n2 12 4 11 3 11 5\n2 11 3 10 2\n1 10 2 9 1\n"
        "1 9 1 8 0\n1 8 0 7 1\n1 7 1 6 2\n1 6 2 5 3\n1 5 3 4 4\n"
        "1 11 5 10 6\n1 10 6 9 7\n1 9 7 8 8\n1 8 8 7 7\n1 7 7 6 6\n"
        "1 6 6 5 5\n1 5 5 4 4\n2 4 4 3 5 3 3\n2 3 5 2 6 2 4\n2 3 3 2 4 2 2\n"
        "1 2 2 1 3\n1 2 6 1 5\n2 2 4 1 5 1 3\n1 1 5 0 4\n1 1 3 0 4";
string const startPos = "TIGER POS\n2 4\nMEN POS\n11 0\n11 1\n11 2\n"
        "11 3\n11 4\n11 5\n11 6\n11 7\n11 8\n12 0\n12 1\n12 2\n"
        "12 3\n12 4\n12 5\n12 6\n12 7\n12 8";
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
