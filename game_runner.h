//
// Created by iancl on 3/25/2018.
//

#ifndef TIGER_GAME_GAME_RUNNER_H
#define TIGER_GAME_GAME_RUNNER_H

#include "function_api.h"
#include <iostream>
#include <map>
#include <list>
class GameRunner{

    vector <Token_t> * gameSTate;
    int square_section_columns,
        square_section_rows;
    map<Point_t, list<Point_t> > * extendedGraph;
    bool isValidMove(vector <Token_t> const &, Move_t) const;

public:
    Color_t playGame();

    //These functions can be used in our AI implementation
    GameRunner(std::istream & graphFile, std::istream & startingPos);
    ~GameRunner(){
        delete this->extendedGraph;
        delete this->gameSTate;
    }
    pair<Point_t *, int> validMoves(vector <Token_t> const &, Token_t);
    bool evaluateWinState( vector <Token_t> &, Color_t & color);

};

bool operator==(Move_t a, Move_t b);
bool operator==(Point_t a, Point_t b);
bool operator==(Token_t a, Token_t b);

#endif //TIGER_GAME_GAME_RUNNER_H
