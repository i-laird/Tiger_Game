/**
 * FILENAME:           constants.h
 * PROJECT:            DEEP BLUE
 * DATE LAST MODIFIED: June 14, 2020
 *
 * copyright 2020
 */

#ifndef TIGER_GAME_CONSTANTS_H
#define TIGER_GAME_CONSTANTS_H

#include <string>
#include "function_api.h"
#include "general_util.h"

// the number of columns on the gameboard
const int NUM_COL = 9;

// the number of rows on the gameboard
// NOTE: the rows within the tiger cage count too
const int NUM_ROW = 13;

const int MAX_CYCLING_CHECK = 4;

const int MAX_RECURSION_DEPTH = 4;

// the max number of turns we allow a game to proceed
const int MAX_TURNS = 250;

// the max number of game states that will be stored at a time
const int HOW_MANY_TO_STORE = 50;

// number that is essentially infinite
// longer than any simple path
const int INFTY = NUM_ROW * NUM_COL + 1;

int const MAX_NUMBER_MOVES = 8;

// used to create the graph
// first is contained the dimensions of the graph
// next are contained diagonal edges
std::string const graph =
                     "BOARD SQUARE SECTION DIMENSIONS\n"
                     "13 9\n"
                     "UNUSUAL EDGES\n"
                     "2 12 4 11 3 11 5\n"
                     "1 11 3 10 2\n"
                     "1 10 2 9 1\n"
                     "1 9 1 8 0\n"
                     "1 8 0 7 1\n"
                     "1 7 1 6 2\n"
                     "1 6 2 5 3\n"
                     "1 5 3 4 4\n"
                     "1 11 5 10 6\n"
                     "1 10 6 9 7\n"
                     "1 9 7 8 8\n"
                     "1 8 8 7 7\n"
                     "1 7 7 6 6\n"
                     "1 6 6 5 5\n"
                     "1 5 5 4 4\n"
                     "2 4 4 3 5 3 3\n"
                     "2 3 5 2 6 2 4\n"
                     "2 3 3 2 4 2 2\n"
                     "1 2 2 1 3\n"
                     "1 2 6 1 5\n"
                     "2 2 4 1 5 1 3\n"
                     "1 1 5 0 4\n"
                     "1 1 3 0 4";

// the starting positions of the tiger and men
// the tiger position is encoded first
// this is followed by the men
std::string const startPos =
                        "TIGER POS\n"
                        "2 4\n"
                        "MEN POS\n"
                        "11 0\n"
                        "11 1\n"
                        "11 2\n"
                        "11 3\n"
                        "11 4\n"
                        "11 5\n"
                        "11 6\n"
                        "11 7\n"
                        "11 8\n"
                        "12 0\n"
                        "12 1\n"
                        "12 2\n"
                        "12 3\n"
                        "12 4\n"
                        "12 5\n"
                        "12 6\n"
                        "12 7\n"
                        "12 8";

// a point that is used to show that the point does not exist
const Point_t NULL_POINT = make_point(-INFTY, -INFTY);

// a token used to show that the given token does not exist
const Token_t NULL_TOKEN = make_token(RED, NULL_POINT);

// a move used to show that there is no move indicated
const Move_t  NULL_MOVE  = make_move(NULL_TOKEN, NULL_POINT);

// the zero vector
const Point_t ZERO_VECT = make_point(0,0);

// point used to signify up
const Point_t UP    = make_point(-1, 0);

// point used to signify down
const Point_t DOWN  = make_point(1, 0);

// point used to signify left
const Point_t LEFT  = make_point(0, -1);

// point used to signify right
const Point_t RIGHT = make_point(0, 1);

const Point_t CAGE_ENTRANCE = make_point(4,4);
const Point_t TIGER_START = make_point(2,4);

/// simple types
typedef vector<Token_t> State;

#endif //TIGER_GAME_CONSTANTS_H
