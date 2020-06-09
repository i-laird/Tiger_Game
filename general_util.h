#include <utility>
#include <vector>
#include <set>
#include <cmath>
#include "function_api.h"

using namespace std;

#ifndef GENERAL_UTIL_H_INCLUDED
#define GENERAL_UTIL_H_INCLUDED

/**********************************            ********************************
*********************************** Point Util ********************************
***********************************            *******************************/

/// Point constructors
/*                       make_point(int, int)
*
 * description: returns a Point_t with the given row and column
 * return: Point_t
 * precondition: none
 * postcondition: the created point is returned
 */
Point_t make_point(int row, int col);

/*                       make_point(const pair<int,int>&)
*
 * description: returns a Point_t with the given row and column
 *              from the pair, the row being first, the col
 *              second
 * return: Point_t
 * precondition: none
 * postcondition: the created point is returned
 */
Point_t make_point(const pair<int,int>& p);

/// Point comparators
/*           operator<(const Point_t& a, const Point_t& b)
*
 * description: returns true iff a has a lesser row than b
 *              or the rows are equal and has a lesser column
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator<(const Point_t& a, const Point_t& b);

/*           operator>(const Point_t& a, const Point_t& b)
*
 * description: returns true iff b < a
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator>(const Point_t& a, const Point_t& b);

/*           operator<=(const Point_t& a, const Point_t& b)
*
 * description: returns true iff a < b or a == b
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator<=(const Point_t& a, const Point_t& b);

/*           operator>=(const Point_t& a, const Point_t& b)
*
 * description: returns true iff a > b or a == b
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator>=(const Point_t& a, const Point_t &b);

/*           operator==(const Point_t&, const Point_t&)
*
 * description: returns true iff the two points have the
 *              same row and column
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator==(const Point_t& a, const Point_t& b);

/*           operator!=(const Point_t& a, const Point_t& b)
 *
 * description: returns true iff a and b have a different row and/or
 *              different column
 * return: bool
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
bool operator!=(const Point_t& a, const Point_t& b);

/// Point operators
/*           operator+(const Point_t&, const Point_t&)
*
 * description: returns a point which is the component-wise sum
 *              of the given points
 * return: Point_t
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
Point_t operator+(const Point_t& a, const Point_t& b);

/*           operator+=(Point_t& a, const Point_t& b)
*
 * description: sets a equal to a + b
 * return: none
 * precondition: the given Point_t objects are valid
 * postcondition: the second Point_t object passed in is unchanged
 *                the first Point_t object passed in is set equal
 *                to the sum of the objects
 */
void operator+=(Point_t& a, const Point_t& b);

/*           operator-(const Point_t&, const Point_t&)
*
 * description: returns a point which is the component-wise difference
 *              of the given points
 * return: Point_t
 * precondition: the given Point_t objects are valid
 * postcondition: the Point_t objects passed in are unchanged
 */
Point_t operator-(const Point_t& a, const Point_t& b);

/*           operator-=(Point_t& a, const Point_t& b)
*
 * description: sets a equal to a - b
 * return: none
 * precondition: the given Point_t objects are valid
 * postcondition: the second Point_t object passed in is unchanged
 *                the first Point_t object passed in is set equal
 *                to the difference of the objects
 */
void operator-=(Point_t& a, const Point_t& b);

/*           operator*(int s, const Point_t& pt)
*
 * description: returns a point whose components are those of pt
 *              multiplied by s
 * return: Point_t
 * precondition: - the given Point_t object is valid
 *               - s and the components of pt are small enough that
 *                 the resulting multiplication is valid
 * postcondition: the Point_t object is unchanged
 */
Point_t operator*(int scalar, const Point_t& vect);

/*           operator/(const Point_t& pt, int s)
*
 * description: returns a point whose components are those of pt divided by
 *              s (note that this is integer division)
 * return: Point_t
 * precondition: - the given Point_t object is valid
 *               - the given divisor is non-zero, NO HANDLING IS PROVIDED
 *                 FOR THE DIVISOR BEING ZERO
 * postcondition: the Point_t object passed in is unchanged
 */
Point_t operator/(const Point_t& vect, int scalar);

/*            abs(const Point_t& pt)
*
 * description: returns a point whose components are the absolute values
 *              of those of pt
 * return: Point_t
 * precondition: the given Point_t object is valid
 * postcondition: the Point_t objects passed in is unchanged
 */
Point_t abs(const Point_t& a);

/*            one_norm(const Point_t& pt)
*
 * description: returns the sum of the absolute values of the components of
 *              pt
 * return: int
 * precondition: the given Point_t object is valid
 * postcondition: the Point_t objects passed in is unchanged
 */
int one_norm(const Point_t& a);


/**********************************            ********************************
*********************************** Token Util ********************************
***********************************            *******************************/

/// Token Constructors
/*            make_token(Color_t c, const Point_t& pt)
*
 * description: returns a Token_t with the given color and point
 * return: Token_t
 * precondition: the given Point_t object is valid
 * postcondition: the Point_t objects passed in is unchanged
 */
Token_t make_token(Color_t c, const Point_t& pt);

/*            make_man(const Point_t& pt)
*
 * description: returns a Token_t with color BLUE and the given point
 *              as its location
 * return: Token_t
 * precondition: the given Point_t object is valid
 * postcondition: the Point_t objects passed in is unchanged
 */
Token_t make_man(const Point_t& pt);

/*            make_tiger(const Point_t& pt)
*
 * description: returns a Token_t with color BLUE and the given point
 *              as its location
 * return: Token_t
 * precondition: the given Point_t object is valid
 * postcondition: the Point_t objects passed in is unchanged
 */
Token_t make_tiger(const Point_t& pt);

/// Token Comparators
/*            operator<(const Token_t& a, const Token_t& b)
*
 * description: true iff a.location < b.location
 * return: bool
 * precondition: - the given Token_t objects are valid
 *               - No two Token_t objects may occupy the same location
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator<(const Token_t& a, const Token_t& b);

/*            operator>const Token_t& a, const Token_t& b)
*
 * description: true iff a.location > b.location
 * return: bool
 * precondition: - the given Token_t objects are valid
 *               - No two Token_t objects may occupy the same location
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator>(const Token_t& a, const Token_t& b);

/*            operator<=(const Token_t& a, const Token_t& b)
*
 * description: true iff a.location < b.location or a.location == b.location
 *              and a.color == b.color
 * return: bool
 * precondition: - the given Token_t objects are valid
 *               - No two Token_t objects may occupy the same location
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator<=(const Token_t& a, const Token_t& b);

/*            operator>=(const Token_t& a, const Token_t& b)
*
 * description: true iff a.location > b.location or a.location == b.location
 *              and a.color == b.color
 * return: bool
 * precondition: - the given Token_t objects are valid
 *               - No two Token_t objects may occupy the same location
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator>=(const Token_t& a, const Token_t &b);

/*            operator==(const Token_t& a, const Token_t& b)
*
 * description: a.location == b.location and a.color == b.color
 * return: bool
 * precondition: the given Token_t objects are valid
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator==(const Token_t& a, const Token_t& b);

/*            operator!=(const Token_t& a, const Token_t& b)
*
 * description: a.location != b.location or a.color == b.color
 * return: bool
 * precondition: the given Token_t objects are valid
 * postcondition: the Token_t objects passed in are unchanged
 */
bool operator!=(const Token_t& a, const Token_t& b);


/**********************************            ********************************
*********************************** Move Util  ********************************
***********************************            *******************************/

/// Move Constructors
/*            make_move(const Token_t& tn, const Point_t& dest)
*
 * description: makes a move with its token a copy of tn and its
 *              destination a copy of dest
 * return: Move_t
 * precondition: the given Token_t object and Point_t object
 *               are valid
 * postcondition: the Token_t object and Point_t objects
 *                passed in are unchanged
 */
Move_t make_move(const Token_t& tn, const Point_t& dest);

/*      make_move_in_direction(const Token_t& tn, const Point_t& dir)
*
 * description: makes a move with its token a copy of tn and its
 *              destination (tn.location + dir)
 * return: Move_t
 * precondition: the given Token_t object and Point_t object
 *               are valid
 * postcondition: the Token_t object and Point_t objects
 *                passed in are unchanged
 */
Move_t make_move_in_direction(const Token_t& tn, const Point_t& direc);

/// Move Comparators
/*            operator==(const Move_t& a, const Move_t& b)
*
 * description: returns true iff a.token == b.token and
 *              a.destination == b.destination
 * return: bool
 * precondition: the given Move_t objects are valid
 * postcondition: the Move_t objects passed in are unchanged
 */
bool operator==(const Move_t &a, const Move_t &b);

/*            operator!=(const Move_t& a, const Move_t& b)
*
 * description: returns true iff a.token != b.token or
 *              a.destination != b.destination
 * return: bool
 * precondition: the given Move_t objects are valid
 * postcondition: the Move_t objects passed in are unchanged
 */
bool operator!=(const Move_t &a, const Move_t& b);

/*            operator<(const Move_t& a, const Move_t& b)
*
 * description: true iff a.token < b.token or a.token == b.token and
 *              a.destination < b.destination
 * return: bool
 * precondition: - the given Move_t objects are valid
 *               - No two Token_t objects may occupy the same location
 * postcondition: the Move_t objects passed in are unchanged
 */
bool operator<(const Move_t& a, const Move_t& b);

/// Move operators
/*            operator-(const Move_t& mv)
*
 * description: returns a Move whose token has the same color as mv.token,
 *              whose destination is mv.token.location, and whose
 *              token location is mv.destination. i.e. returns a move
 *              which undoes mv
 * return: Move_t
 * precondition: the given Move_t object is valid
 * postcondition: the Move_t objects passed in is unchanged
 */
Move_t operator-(const Move_t& m);

/*            operator+=(Move_t& a, const Move_t& b)
*
 * description: - sets the location of the token of a to
 *                the sum of the location of the token of b with that of a
 *              - sets the location of the destination of a to
 *                the sum of the destination of b with that of a
 * return: none
 * precondition: the given Move_t objects are valid
 * postcondition: - the second Move_t objects passed in is unchanged
 *                - the first Move_t object's token location and destination
 *                  are each set to the respective sum with the second Move_t
 *                  object's members
 */
void operator+=(Move_t &a, const Move_t& b);

/*            operator+(Move_t& a, const Move_t& b)
*
 * description: returns a Move_t object whose token location is the sum of the
 *              token locations of a and b, and whose destination is the
 *              sum of the destinations of a and b
 * return: Move_t
 * precondition: the given Move_t objects are valid
 * postcondition: the given Move_t objects passed in are unchanged
 */
Move_t operator+(const Move_t& a, const Move_t& b);

/*            one_norm(const Move_t& mv)
*
 * description: returns the one_norm of the point
 *              (mv destination - mv token location)
 * return: int
 * precondition: the given Move_t objects is valid
 * postcondition: the Move_t object passed in is unchanged
 */
int one_norm(const Move_t & m);


/***************************** constants and simple types *********************/
/// constants TODO
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

const Point_t CAGE_ENTRANCE = make_point(4,4);
const Point_t TIGER_START = make_point(2,4);

/// simple types
typedef vector<Token_t> State;


#endif // GENERAL_UTIL_H_INCLUDED
