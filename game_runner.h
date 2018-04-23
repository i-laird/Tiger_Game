//
// Created by iancl on 3/25/2018.
//

#ifndef TIGER_GAME_GAME_RUNNER_H
#define TIGER_GAME_GAME_RUNNER_H

#include "function_api.h"
#include "general_util.h"
#include "Unordered_State.h"
#include <iostream>
#include <map>
#include <list>
#include <sstream>

Move_t  Move_Deep_Blue(vector<Token_t>, Color_t turn);

int const MAX_NUMBER_MOVES = 8;
string const graph = "BOARD SQUARE SECTION DIMENSIONS\n13 9\nUNUSUAL EDGES"
                     "\n2 12 4 11 3 11 5\n1 11 3 10 2\n1 10 2 9 1\n"
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
    int col_boundary,
        row_boundary,
        tiger_cage_row_offset;
    map<Point_t, list<Point_t>> * extendedGraph;
    /*
     * createGraph
     * precondition: none
     * postcondition: the internal graph for diagonal edges is created
     * return: void
     */
    void createGraph(std::istream & graphFile, std::istream & boardLayout);
public:
    vector <Token_t> * gameState;
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
    /**
     * Description: Sees is the indicated move is valid. i.e. Can the indicated
     *              token move to the position specified.
     *              return: true means valid move, false means invalid
     * precondition: game state is valid
     * postcondition: game is unchanged
     * return: First pair element indicates if a player won, second pair element
     *             indicates which player if a win occurred
     */
    bool isValidMove(Unordered_State const &, Move_t) ;
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
     * description: Default Constructor
     *              Reads in game board from string streams.
     *              This is the one that should be used in our AI
     * precondition: GameRunner does not exist
     * Return: none: GameRunner is created
     */
    GameRunner();
    /*
     * description: Custom Constructor
     *              Reads in game board and starting positions for pieces
     *              from files. This is the one that is used for testing reasons.
     * precondition: GameRunner does not exist
     * Return: none: GameRunner is created
     */
    GameRunner(std::istream & graphFile, std::istream & startingPos);
    /*
     * description: Custom Constructor
     *              Reads in game board from string streams.
     *              Allows custom starting pos
     * precondition: GameRunner does not exist
     * Return: none: GameRunner is created
     */
    GameRunner(std::istream & startingPos);
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
    pair<Point_t *, pair<bool *, int> >  validMoves(vector <Token_t> const &, Token_t);
    /*
     * description: returns all Moves a Token can make.
     *                  will return Tiger jump moves.
     * precondition: Board state is valid
     * postcondition: Board is unchanged.
     * Return: a pointer to the Moves along with the # of moves.
     */
    pair<Point_t *, pair<bool *, int> >  validMoves(Unordered_State const &, Token_t);
    /*
     * description: sees if a player has won (Tiger or Men)
     * precondition: Board state is valid
     * postcondition:  Board is unchanged
     * return: true is a player has won(player is stored in refernce variable)
     */
    bool evaluateWinState( vector <Token_t> &, Color_t & color);

    /*
     * BFS_To_Point
     * description: finds the shortest path to the indicated point
     * precondition:
     * postcondition: the game and tokens are unchanged
     * return: if a path is found the point that should be moved to is returned
     *     boolean passed by reference is updated
     */
    Point_t BFS_To_Point(vector<Token_t> mapLayout, int, Point_t moveTo, Color_t, bool &);

    /*
     * tiger_move
     * description: calculates the Tiger move for the indicated game state
     * precondition: game state is valid and a move can be made
     * postcondition: game and tokens are unchanged
     * return: the move that should be made
     */
    Move_t Tiger_Move(vector<Token_t> &, int randomProbability = 0);

    /*
     * getRandomizer
     * description: calculates how random the tiger move should be
     * precondition: game state is valid
     * return: number between 0 and 3 indicating how random the Tiger should be
     *      -o means 0% random
     */
    int getRandomizer(vector<Token_t> &);

    /*    Undeterministic_Tiger_Move
     * description: calculates a Tiger move but will also inject randomness
     * precondition: game state is valid and tiger can move
     * postcondition: tiger move is calculated
     * return: the tiger move that should be made
     */
    Move_t Undeterministic_Tiger_Move(vector<Token_t> &);

};


#endif //TIGER_GAME_GAME_RUNNER_H
