/**
 * FILENAME:           game_runner.h
 * PROJECT:            TIGER PRIME
 * DATE LAST MODIFIED: June 14, 2020
 *
 * copyright 2020
 */

#ifndef TIGER_GAME_GAME_RUNNER_H
#define TIGER_GAME_GAME_RUNNER_H

#include "Unordered_State.h"
#include "constants.h"
#include <iostream>
#include <map>
#include <list>
#include <sstream>

Move_t  Move_Deep_Blue(vector<Token_t>, Color_t turn);

/**
 * GameRunner
 *
 * Is in complete control of the game. Keeps track of the current
 * state of the board. Calculates when either the Tiger or Men have won,
 * and all possible moves.
 *
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
     * Description: Sees is the indicated move is valid. i.e. Can indicated
     *              token move to the position specified.
     *              return: true means valid move, false means invalid
     * precondition: game state is valid
     * postcondition: game is unchanged
     * return: First pair element indicates if a player won, second element
     *             indicates which player if a win occurred
     */
    bool isValidMove(vector <Token_t> const &, Move_t) ;

    /**
     * Description: Sees is the indicated move is valid. i.e. Can indicated
     *              token move to the position specified.
     *              return: true means valid move, false means invalid
     * precondition: game state is valid
     * postcondition: game is unchanged
     * return: First pair element indicates if a player won, second element
     *             indicates which player if a win occurred
     */
    bool isValidMove(Unordered_State const &, Move_t) ;

    /**
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
     *              from files. This is the one that is used testing reasons.
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
    pair<Point_t *, pair<bool *, int> >  validMoves(vector <Token_t> const &,
                                                    Token_t);
    /*
     * description: returns all Moves a Token can make.
     *                  will return Tiger jump moves.
     * precondition: Board state is valid
     * postcondition: Board is unchanged.
     * Return: a pointer to the Moves along with the # of moves.
     */
    pair<Point_t *, pair<bool *, int> >  validMoves(Unordered_State const &,
                                                    Token_t);
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
    Point_t BFS_To_Point(vector<Token_t> mapLayout, int, Point_t moveTo,
                         Color_t, bool &);

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
