#include <utility>
#include <vector>
#include <set>
#include <cmath>
#include "function_api.h"

using namespace std;

#ifndef GENERAL_UTIL_H_INCLUDED
#define GENERAL_UTIL_H_INCLUDED

/**********************************            *********************************
*********************************** Point Util *********************************
***********************************            ********************************/

/// Point constructors
Point_t make_point(int row, int col);

Point_t make_point(pair<int,int>& p);

/// Point comparators
bool operator<(const Point_t& a, const Point_t& b);

bool operator>(const Point_t& a, const Point_t& b);

bool operator<=(const Point_t& a, const Point_t& b);

bool operator>=(const Point_t& a, const Point_t &b);

bool operator==(const Point_t& a, const Point_t& b);

bool operator!=(const Point_t& a, const Point_t& b);

/// Point operators
Point_t operator+(const Point_t& a, const Point_t& b);

void operator+=(Point_t& a, const Point_t& b);

Point_t operator-(const Point_t& a, const Point_t& b);

void operator-=(Point_t& a, const Point_t& b);

Point_t operator*(int scalar, const Point_t& vect);

Point_t operator/(const Point_t& vect, int scalar);

Point_t abs(const Point_t& a);

int one_norm(const Point_t& a);


/**********************************            *********************************
*********************************** Token Util *********************************
***********************************            ********************************/

/// Token Constructors

Token_t make_token(Color_t c, const Point_t& pt);

Token_t make_man(const Point_t& pt);

Token_t make_tiger(const Point_t& pt);

/// Token Comparators
bool operator<(const Token_t& a, const Token_t& b);

bool operator>(const Token_t& a, const Token_t& b);

bool operator<=(const Token_t& a, const Token_t& b);

bool operator>=(const Token_t& a, const Token_t &b);

bool operator==(const Token_t& a, const Token_t& b);

bool operator!=(const Token_t& a, const Token_t& b);


/**********************************            *********************************
*********************************** Move Util  *********************************
***********************************            ********************************/

/// Move Constructors
Move_t make_move(const Token_t& tn, const Point_t& dest);

Move_t make_move_in_direction(const Token_t& tn, const Point_t& direc);

/// Move Comparators
/*
 * description: operator==
 * return: true is the two Move_t are equal
 */
bool operator==(const Move_t &a, const Move_t &b);

bool operator!=(const Move_t &a, const Move_t& b);

/// Move operators
Move_t operator-(const Move_t& m);

void operator+=(Move_t &a, const Move_t& b);

Move_t operator+(const Move_t& a, const Move_t& b);

int one_norm(const Move_t & m);

/***************************** additional operators ***************************/
// returns new set with given columns moved in given position
set<Point_t> move_cols(const Point_t& direc, const set<Point_t>& pts,
                       set<int> cols);

/***************************** constants and simple types *********************/
/// constants
const int NUM_COL = 9;
const int NUM_ROW = 13;

const int INFTY = NUM_ROW * NUM_COL + 1; // longer than any simple path

const Point_t NULL_POINT = make_point(-INFTY, -INFTY);
const Token_t NULL_TOKEN = make_token(RED, NULL_POINT);
const Move_t  NULL_MOVE  = make_move(NULL_TOKEN, NULL_POINT);

const Point_t ZERO_VECT = make_point(0,0);
const Point_t UP    = make_point(-1, 0);
const Point_t DOWN  = make_point(1, 0);
const Point_t LEFT  = make_point(0, -1);
const Point_t RIGHT = make_point(0, 1);


/// simple types
typedef vector<Token_t> State;

#endif // GENERAL_UTIL_H_INCLUDED
