//
// Created by ian on 6/8/20.
//

#ifndef TIGER_GAME_CONSTANTS_H
#define TIGER_GAME_CONSTANTS_H

#include <string>
#include "function_api.h"
#include "general_util.h"

const int NUM_COL = 9;
const int NUM_ROW = 13;
const int MAX_CYCLING_CHECK = 4;
const int MAX_RECURSION_DEPTH = 4;
const int INFTY = NUM_ROW * NUM_COL + 1; // longer than any simple path
int const MAX_NUMBER_MOVES = 8;
std::string const graph = "BOARD SQUARE SECTION DIMENSIONS\n13 9\nUNUSUAL EDGES"
                     "\n2 12 4 11 3 11 5\n1 11 3 10 2\n1 10 2 9 1\n"
                     "1 9 1 8 0\n1 8 0 7 1\n1 7 1 6 2\n1 6 2 5 3\n1 5 3 4 4\n"
                     "1 11 5 10 6\n1 10 6 9 7\n1 9 7 8 8\n1 8 8 7 7\n1 7 7 6 6\n"
                     "1 6 6 5 5\n1 5 5 4 4\n2 4 4 3 5 3 3\n2 3 5 2 6 2 4\n2 3 3"
                     " 2 4 2 2\n"
                     "1 2 2 1 3\n1 2 6 1 5\n2 2 4 1 5 1 3\n1 1 5 0 4\n1 1 3 0 4";
std::string const startPos = "TIGER POS\n2 4\nMEN POS\n11 0\n11 1\n11 2\n"
                        "11 3\n11 4\n11 5\n11 6\n11 7\n11 8\n12 0\n12 1\n12 2\n"
                        "12 3\n12 4\n12 5\n12 6\n12 7\n12 8";

const Point_t NULL_POINT = make_point(-INFTY, -INFTY);
const Token_t NULL_TOKEN = make_token(RED, NULL_POINT);
const Move_t  NULL_MOVE  = make_move(NULL_TOKEN, NULL_POINT);

const Point_t ZERO_VECT = make_point(0,0);
const Point_t UP    = make_point(-1, 0);
const Point_t DOWN  = make_point(1, 0);
const Point_t LEFT  = make_point(0, -1);
const Point_t RIGHT = make_point(0, 1);

const Point_t CAGE_ENTRANCE = make_point(4,4);
const Point_t TIGER_START = make_point(2,4);

/// simple types
typedef vector<Token_t> State;

#endif //TIGER_GAME_CONSTANTS_H
