/*
* Author: Ben Sepanski, Ian laird, J.R. Diehl, Wyatt Snyder, Aidan Edwards
* Assignment Title: Deep Blue Tiger Game AI
* Assignment Description: This program can play as both the Tiger and men
* Due Date: 4/27/2018
* Date Created: 4/24/2018
* Date Last Modified: 4/25/2018
 */

#ifndef DEEP_BLUE_GROUP_PROJECT_H
#define DEEP_BLUE_GROUP_PROJECT_H
//Stub this out for submission
#include "function_api.h"

#include <iostream>
#include <map>
#include <list>
#include <sstream>
#include <set>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;

/*
 * move_deep_blue
 * description: this will calculated the best move for the indicated color
 * precondition: game state is valid
 * postcondition: game is unchanged
 * return: optimal move
 */
Move_t  Move_Deep_Blue(vector<Token_t>, Color_t turn);

/// constants
const int NUM_COL = 9;
const int NUM_ROW = 13;
const int MAX_CYCLING_CHECK = 4;
const int MAX_RECURSION_DEPTH = 4;
const int INFTY = NUM_ROW * NUM_COL + 1; // longer than any simple path
int const MAX_NUMBER_MOVES = 8;
string const graph = "BOARD SQUARE SECTION DIMENSIONS\n13 9\nUNUSUAL EDGES"
                     "\n2 12 4 11 3 11 5\n1 11 3 10 2\n1 10 2 9 1\n"
                     "1 9 1 8 0\n1 8 0 7 1\n1 7 1 6 2\n1 6 2 5 3\n1 5 3 4 4\n"
                   "1 11 5 10 6\n1 10 6 9 7\n1 9 7 8 8\n1 8 8 7 7\n1 7 7 6 6\n"
                     "1 6 6 5 5\n1 5 5 4 4\n2 4 4 3 5 3 3\n2 3 5 2 6 2 4\n2 3 3"
                     " 2 4 2 2\n"
                   "1 2 2 1 3\n1 2 6 1 5\n2 2 4 1 5 1 3\n1 1 5 0 4\n1 1 3 0 4";
string const startPos = "TIGER POS\n2 4\nMEN POS\n11 0\n11 1\n11 2\n"
                        "11 3\n11 4\n11 5\n11 6\n11 7\n11 8\n12 0\n12 1\n12 2\n"
                        "12 3\n12 4\n12 5\n12 6\n12 7\n12 8";

/// simple types
typedef vector<Token_t> State;

/// State that is not affected by order of men tokens
class Unordered_State{
private:
    Token_t tiger;
    set<int> col_to_rows[NUM_COL]; // the ith set
    // holds the set of rows containing men in the ith column
public:
    /// constructors
    /*       Unordered_State()
     *
     * description: default constructor, sets members to defaults
     * return: none
     * precondition: none
     * postcondition: an Unordered_State is created
     */
    Unordered_State();
    /// assignment operator
    /*       operator=
     *
     * description: assignment operator, makes deep copy and returns a const
     *              reference to this object
     * return: const Unordered_State&
     * precondition: that object is a valid Unordered State
     * postcondition: this Unordered_State is a copy of that
     */
    const Unordered_State& operator=(const Unordered_State& that);

    /*       Unordered_State(const State& st)
     *
     * description: constructor, sets tiger to the first Token_t in the
     *              State, and creates the associate col_to_row map
     *              so that it describes the men positions
     * return: none
     * precondition: - The given State is valid
     *               - The first Token_t in State is the tiger, the
     *                 rest are men
     *               - Each Token_t in State is in a valid state on
     *                 the board
     * postcondition: an Unordered_State is created
     */
    Unordered_State(const State& st);
    /// comparators
    /*       operator<
     *
     * description: returns true iff this Unordered_State has less
     *              men than that Unordered_State, or they have the
     *              same number of men, but the first differing
     *              location a amongst their respective tiger and
     *              men locations has this.a < that.a
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator<(const Unordered_State& that) const;

    /*       operator>(const Unordered_State& that)
     *
     * description: returns true iff that < *this
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator>(const Unordered_State& that) const;

    /*       operator<=(const Unordered_State& that)
     *
     * description: returns true iff *this < that or *this == that
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator<=(const Unordered_State& that) const;

    /*       operator>=(const Unordered_State& that)
     *
     * description: returns true iff *this > that or *this == that
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator>=(const Unordered_State& that) const;

    /*       operator==(const Unordered_State& that)
     *
     * description: returns true iff this men locations equal that
     *              men locations, and this tiger == that tiger
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator==(const Unordered_State& that) const;

    /*       operator!=(const Unordered_State& that)
     *
     * description: returns true iff *this is not equal to that
     * return: bool
     * precondition: that Unordered_State object is valid
     * postcondition: this object and that object are unchanged
     */
    bool operator!=(const Unordered_State& that) const;

    /// accessors

    /*       get_tiger
     *
     * description: returns this tiger
     * return: const Token_t&
     * precondition: this Unordered_State object is valid
     * postcondition: this object is unchanged
     */
    const Token_t& get_tiger() const;

    /*       rows_in_col(int col)
     *
     * description: returns the set of rows occupied by men in the given
     * col
     * return: const set<int>&
     * precondition: this Unordered_State object is valid
     * postcondition: this object is unchanged
     */
    const set<int>& rows_in_col(int col) const;
    /// mutators
    /*       set_tiger(const Token_t& t)
     *
     * description: sets this tiger to a copy of t
     * return: none
     * precondition: this Unordered_State object is valid
     * postcondition: this object's tiger is set to t
     */
    void set_tiger(const Token_t& t);

    /*       set_men_locs(const set<Point_t>& locs)
     *
     * description: sets this col_to_row to match the given set of locations
     * return: none
     * precondition: this Unordered_State object is valid
     * postcondition: this object's men locations represent locs
     */
    void set_men_locs(const set<Point_t>& locs);

    /// operators
    /*                   do_move
    *
    * description: If given Move_t moves an existing token to an unoccupied
    *              position, perform the move
    * return: bool
    * precondition: This is a valid Unordered_State object
    * postcondition: - returns false if the given Move is attempting to move
    *                  a non-existent token or trying to move to an occupied
    *                  position.
    *                - else returns true and performs the move
    */
    bool do_move(const Move_t& m);

    /// functions to convenience getting information
    /*                   is_occupied
    *
    * description: Given a point returns true iff the point is occupied
    * return: bool
    * precondition: This is a valid Unordered state object
    * postcondition: - returns true iff the point is occupied by the tiger
    *                  or a man
    */
    bool is_occupied(const Point_t& pt) const;

    /*       State()
     *
     * description: implicit type constructor, returns a State object
     *              whose first entry is the tiger token, followed by
     *              a men tokens containing each men positions in this
     *              state
     * return: State
     * precondition: none
     * postcondition: this object is unchanged
     */
    operator State() const;

};

/**
 * This class runs a game. It can be used like Booth's API to fetch moves
 * and run them on the Game
 *
 * However, It can also be used in our program to find moves of a piece because
 * it knows the board layout.
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


/***************************** constants and simple types ********************/


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

class Men_Mover {
protected:
    // about current state
    GameRunner game;
    Unordered_State current;
public:
    /*       Men_Mover(const State& s)
     *
     * description: creates a Men_Mover object, sets current to s
     * return: none
     * precondition: s is a valid State
     * postcondition: a Men_Mover object is created
     */
    Men_Mover(const State &s);

    /*       next_move(State s)
     *
     * description: returns the Move_t which the men should perform in the
     *              given state
     * return: Move_t
     * precondition: s is a valid State
     * postcondition: this current is set to s
     */
    virtual Move_t next_move(State s);

    /*       ~Men_Mover()
     *
     * description: destructor
     * return: none
     * precondition: none
     * postcondition: this object is deleted
     */
    virtual ~Men_Mover() = default;
protected:
    /*       execute_move()
     *
     * description: returns the Move_t which the men should perform given
     *              that current is the current state
     * return: Move_t
     * precondition: current is a valid State
     * postcondition: none
     */
    virtual Move_t execute_move() = 0;
};


// these points are those which should be occupied in order to
// keep the tiger in the cage
const int STAGE_POS_SIZE = 8;
const Point_t STAGE_POSITIONS[STAGE_POS_SIZE] = { make_point(4, 3),
                                                  make_point(4, 4),
                                                  make_point(4, 5),
                                                  make_point(5, 3),
                                                  make_point(5, 4),
                                                  make_point(5, 5),
                                                  make_point(6,2),
                                                  make_point(6,6)};

typedef pair<int,int> Hash_val;

/*            hash_locs(const Unordered_State& st, int back_row)
*
 * description: returns a Hash_val uniquely associated to a set
 *              of men positions in the rows [back_row, back_row - 4)
 * return: Hash_val
 * precondition: the given Unordered_State is valid and all of the
 *               men in the state are in the rows [back_row, back_row - 4)
 * postcondition: the given Unorderd_State is unchanged
 */
Hash_val hash_locs(const Unordered_State& st, int back_row);

/*        next_hash(const Move_t& m, Hash_val old_hash, int back_row)
*
 * description: returns a Hash_val uniquely associated to a set
 *              of men positions in the rows [back_row, back_row - 4)
 *              created by performing the given Move_t on the set of
 *              men_positions associated to the previous Hash_val
 * return: Hash_val
 * precondition: - the given Move_t is valid, the given old_hash is valid,
 *                 and before and after the move the associated board
 *                 positions have men locations in [back_row, back_row - 4)
 *               - the Move_t is moving a MAN
 * postcondition: the given Move_t object is unchanged
 */
Hash_val next_hash(const Move_t& m, Hash_val old_hash, int back_row);

/*       tiger_can_jump(const Unordered_State * st, GameRunner *g)
 *
 * description: returns true iff the tiger can jump in the given state
 * return: bool
 * precondition: the given Unordered_State* points to a valid Unordered_State
 *               the given GameRunner* points to a valid GameRunner
 * postcondition: the given Unorderd_State and GameRunner pointed to
 *                are unchanged
 */
bool tiger_can_jump(const Unordered_State * st, GameRunner *g);

/*       secure(const Unordered_State * st, GameRunner *g, Move_t off_move)
 *
 * description: returns true iff the tiger could jump given an arbitrary number
 *              of moves starting from its starting position
 *              If the off_move given is valid, returns true iff the tiger
 *              could jump given an arbitrary number of moves starting from the
 *              tiger position with the off_move undone
 * return: bool
 * precondition: - the given Unordered_State* points to a valid Unordered_State
 *               - the given GameRunner* points to a valid GameRunner
 *               - no man is in the tiger start position TIGER_START
 * postcondition: the given Unorderd_State and GameRunner pointed to
 *                are unchanged
 */
bool secure(Unordered_State* st, GameRunner* g, Move_t off_move = NULL_MOVE);

class Specific_Move_Handler {
private:
    Unordered_State * current;
    GameRunner* g;
    queue<Move_t> response;
    int front_row, back_row;
    Move_t off_move;

    /*          one_col_two_back()
     *
     * description: Returns true if current is in the case where all
     *              columns are in the same two rows except for one column which
     *              is lagging to columns back. If in the case, adds the moves
     *              to do to response
     * return: bool
     * precondition: none
     * postcondition: If in this special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool one_col_two_back();

    /*          three_by_diag()
     *
     * description: Returns true if current is in the in the case
     *              where three columns with a diagonal through the left or
     *              right column are  being held down by the tiger.
     *              If in the case, adds the moves to do to response
     * return: bool
     * precondition: none
     * postcondition: If in this special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool three_by_diag();

    /*          near_top_finish_three_by_diag()
     *
     * description: Returns true if current is in the in the case
     *              where three columns with a diagonal through the left or
     *              right column were being held down by the tiger, but the
     *              tiger has left and the three_by_diag() case needs to be
     *              finished.
     *              If in the case, adds the moves to do to response
     * return: bool
     * precondition: none
     * postcondition: If in this special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool near_top_finish_three_by_diag();

    /*          lagging_col()
     *
     * description: Returns true if current is in the in the case
     *              where one column is lagging behind all the others and in
     *              the middle column if on the lower half of the board, or
     *              one of the edge columns if on the upper half.
     *              If in the case, adds the moves to do to response
     * return: bool
     * precondition: none
     * postcondition: If in this special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool lagging_col();

    /*          try_switch()
     *
     * description: Returns true if current is in the in the case
     *              where one column is lagging behind all the others but not
     *              in the middle column if on the lower half of the board, or
     *              not on  one of the edge columns if on the upper half.
     *              If in the case, adds the moves to do to response to try
     *              to shift the lag to the appropriate column
     * return: bool
     * precondition: none
     * postcondition: If in this special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool try_switch();
public:
    /*         Specific_Move_Handler(Unordered_State*, GameRunner* g)
     *
     * description: default constructor, creates a Specific_Move_Handler
     * return: none
     * precondition: none
     * postcondition: A Specific_Move_Handler object is created
     */
    Specific_Move_Handler(Unordered_State* = nullptr, GameRunner* g = nullptr);

    /*         set_front_back_row(int front_row, int back_row)
     *
     * description: mutator function, sets front and back row to given values
     * return: none
     * precondition: none
     * postcondition: front_row and back_row in this object are set to the
     *                given values
     */
    void set_front_back_row(int front_row, int back_row);

    /*         set_front_back_row(int front_row, int back_row)
     *
     * description: Returns true if in one of the special cases detailed
     *              above in the private member functions. If in one of these
     *              cases, response is filled with the appropriate men response
     * return: bool
     * precondition: none
     * postcondition: If in a special state, response is set to the series
     *                of moves that the men should take in response to this
     *                case
     */
    bool handle_special_case(Move_t off_move);

    /*              clear_moves()
     *
     * description: clears response
     * return: none
     * precondition: none
     * postcondition: response is set to an empty queue
     */
    void clear_moves();

    /*              get_move()
     *
     * description: returns move on front of response and pops it from the
     *              queue, returns NULL_MOVE if response is empty
     * return: Move_t
     * precondition: none
     * postcondition: if response was not empty, its front entry is popped
     */
    Move_t get_move();
};

class Smart_Mover : public Men_Mover {
private:
    // fun times with queues
    queue<Move_t> q;
    list<Move_t> prev_moves;

    // about current state
    int front_row;
    int back_row;

    // about next move
    bool move_ready;
    map<pair<Hash_val,Token_t>, Move_t> path;
    set<Hash_val> desired;

    Specific_Move_Handler special_moves;

    // about off move
    bool off_move_ready;
    bool off_move_active;
    Move_t off_move;

    // whether or not to tiger cage
    bool to_tiger_cage;

    /*       determine_rows()
     *
     * description: determines this back and front row
     *              - back_row is the row of the furthest back row
     *              - front_row is back_row - 3 or back_row - 4 if only one
     *                column is lagging in either the edge if front_row is
     *                on the top half of the board, or the middle if
     *                on the bottom half
     * return: void
     * precondition: none
     * postcondition: this back_row and front_row are set
     */
    void determine_rows();

    /*       find_moves_to_do()
     *
     * description: - returns nullptr if more than two men are in any
     *                column
     *              - else, returns an array of arrays size 2 which
     *                are the respective Move_t objects to move the
     *                men in that column up towards the front_row
     *                if neither is two in front of the either
     *                    - a NULL_MOVE means no Move_t is left to
     *                      be done for the respective man in that column
     *                    - Has the move with lowest row first in each
     *                      column
     * return: Move_t**
     * precondition: none
     * postcondition: this object is unchanged
     */
    Move_t** find_moves_to_do();

    /*       try_for_specific_move()
     *
     * description: sees if in a specific move case, and if so
     *              returns that Move, else returns NULL_MOVE
     * return: Move_t
     * precondition: none
     * postcondition: If a special move is found, clears this path and
     *                desired and sets move_ready to false
     */
    Move_t try_for_specific_move();

    /*       off_move_handling()
     *
     * description: if man in danger because of off_move, undoes it
     *              else if off_move_active but no off move exists,
     *              perform an off move
     *              else if off_move_active is false but an off move does
     *              exist, undoes the off move
     * return: Move_t
     * precondition: none
     * postcondition: If an off move is to be done, sets off_move_ready,
     *                clears this path, desired, move_ready, and
     *                special_moves
     */
    Move_t off_move_handling();

    /*       search_for_state()
     *
     * description: searches for guaranteed paths on each set of adjacent
     *              3 columns to secure states which involve either 1 or 2
     *              of the given columns being moved up
     * return: none
     * precondition: none
     * postcondition: If a guaranteed path is found, sets this path to that
     *                path and move_ready to true
     */
    void search_for_state();

    /*       safety_fail_safe()
     *
     * description: If the suggested move produces a state from which the
     *              tiger can capture, sees if there is a different
     *              move from which the tiger cannot capture, and returns
     *              that
     * return: Move_t
     * precondition: none
     * postcondition: this object is unchanged
     */
    Move_t safety_fail_safe(Move_t suggested);

    /*       fail_safe()
     *
     * description: Performs safety_fail_safe(), then if the suggested
     *              move is invalid, searches for a move after which
     *              the tiger cannot capture and returns it. If no such
     *              move exists, searches for a valid move, and returns one
     * return: Move_t
     * precondition: none
     * postcondition: this object is unchanged
     */
    Move_t fail_safe(Move_t suggested);

    /*       leads_to_cycle(Move_t mv)
     *
     * description: If to tiger cage, returns true iff mv returns the men
     *              locations to a previous state visited in the last
     *              MAX_CYCLING_CHECK moves, else returns false
     *
     * return: bool
     * precondition: none
     * postcondition: this object is unchanged
     */
    bool leads_to_cycle(Move_t mv);

    /*       tiger_reachable_pos()
     *
     * description: Returns the set of points which the tiger could reach
     *              in an arbitrary number of moves, not including its
     *              current position
     * return: set<Point_t>
     * precondition: none
     * postcondition: this object is unchanged
     */
    set<Point_t> tiger_reachable_pos();

    /*       finish_off_tiger()
     *
     * description: Returns a move to do for when at tiger cage
     * return: Move_t
     * precondition: the only reachable positions from the tiger include the
     *               row in front of the tiger cage and the tiger cage itself
     * postcondition: this object is unchanged
     */
    Move_t finish_off_tiger();

public:
    /*       Smart_Mover(const State& s)
     *
     * description: creates a Smart_Mover object with the given State
     *              used to create current
     * return: none
     * precondition: s is a valid State
     * postcondition: a Smart_Mover object is created
     */
    Smart_Mover(const State& s);
    ~Smart_Mover(){}

protected:
    /*       execute_move()
     *
     * description: returns the best move for the men to perform
     *              in order to prevent capture of men and
     *              move men forward
     * return: Move_t
     * precondition: none
     * postcondition: this object is unchanged
     */
    Move_t execute_move() override;
};



class Transition {
private:
    Unordered_State* current;
    set<Hash_val>* hash_vals; // hashes of desired states
    GameRunner * game;
    int back_row;
    pair<int,int> left_r_bds;
    pair<int,int> low_high_bds;
    map<pair<Hash_val,Token_t>, pair<int,bool>> visited;// map to num moves
    // this state was last seen at and whether or not was true
    map<pair<Hash_val,Token_t>, Move_t> path;
    Hash_val cur_hash_val;

    /*       search_for_state(int max_moves, int num_moves)
     *
     * description: recursively searches for a guaranteed path to one of the
     *              desired states with maximum number of moves of max_moves
     *              returns true iff a path is found from this state
     * return: bool
     * precondition: none
     * postcondition: the path to the desired states is stored in path
     *                if such a path is found
     */
    bool search_for_state(int max_moves, int num_moves = 0);
public:
    /*                          Transition
     *
     * description: creates a Transition object, using the given values to
     *              set current, hash_vasl, game, back_row, and the
     *              left_r_bds as well as low_high_bds
     * return: none
     * precondition: the given parameters are valid objects/pointers to
     *               valid objects
     * postcondition: a Transition object is created
     */
    Transition(Unordered_State* c, set<Hash_val>*hash_vals,GameRunner*game,
               int back_row, pair<int,int> left_right_bds,
               pair<int,int> low_high_bds);

    /*       find_path_to_state(int max_moves, int minimum_max_moves)
     *
     * description: searches for a guaranteed path to one of the desired
     *              states with maximum number of moves of minimum_max_moves
     *              up to max_moves
     *                 returns true iff a path is found
     * return: bool
     * precondition: none
     * postcondition: the path to the desired states is stored in path
     *                if such a path is found, the set of visited
     *                states is cleared and then updated
     */
    bool find_path_to_state(int max_moves, int minimum_max_moves = 1);

    /*       get_path
     *
     * description: returns a copy of this path
     * return: map<pair<Hash_val,Token_t>, Move_t>
     * precondition: none
     * postcondition: this object is unchanged
     */
    map<pair<Hash_val,Token_t>, Move_t> get_path();
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/// Unordered_State constructors
Unordered_State::Unordered_State() {
    this->tiger = NULL_TOKEN;
    for (auto &col_to_row : col_to_rows) {
        col_to_row = set<int>();
    }
}


/// assignment operator
const Unordered_State& Unordered_State::operator=(const Unordered_State& that){
    if(this != &that) {
        this->tiger = that.tiger;
        for(int c = 0; c < NUM_COL; ++c) {
            this->col_to_rows[c] = set<int>();
            for(auto r = that.col_to_rows[c].begin();
                r != that.col_to_rows[c].end(); ++r) {
                col_to_rows[c].insert(*r);
            }
        }
    }

    return *this;
}

Unordered_State::Unordered_State(const State& st) {
    for (auto &col_to_row : col_to_rows) {
        col_to_row = set<int>();
    }
    // look at each token in State, if tiger store in tiger,
    // if a man add its location to the set of men
    for (auto man : st) {
        bool valid = true;
        // index protection
        if(man.location.col < 0 || man.location.col >= NUM_COL) {
            valid = false;
        }
        if(man.color == RED && valid) {
            this->tiger = man;
        }
        else if(man.color == BLUE && valid) {
            this->col_to_rows[man.location.col].insert(man.location.row);
        }
    }
}


/// Comparators
bool Unordered_State::operator<(const Unordered_State& that) const {
    bool lessThan = false;
    //compare tiger
    if(this->tiger < that.tiger) {
        lessThan = true;
    }// if same tiger, compare men locs
    else if(this->tiger == that.tiger) {
        bool determined = false;
        for(int i = 0 ; i < NUM_COL && !determined; ++i) {
            if(this->col_to_rows[i] < that.col_to_rows[i]) {
                determined = true;
                lessThan = true;
            }
            else if(this->col_to_rows[i] > that.col_to_rows[i]) {
                determined = true;
                lessThan = false;
            }
        }
    }


    return lessThan;
}


bool Unordered_State::operator>(const Unordered_State& that) const {
    return (that < *this && that != *this);
}


bool Unordered_State::operator<=(const Unordered_State& that) const {
    return (*this < that) || (*this == that);
}


bool Unordered_State::operator>=(const Unordered_State& that) const {
    return (*this > that) || (*this == that);
}


bool Unordered_State::operator==(const Unordered_State& that) const {
    bool equals = true;
    if(this->tiger != that.tiger) {
        equals = false;
    }
    else {
        for(int i = 0 ; i < NUM_COL && equals; ++i) {
            if(this->col_to_rows[i] != that.col_to_rows[i]) {
                equals = false;
            }
        }
    }
    return equals;
}


bool Unordered_State::operator!=(const Unordered_State& that) const {
    return !(*this == that);
}


/// Accessor functions
const Token_t& Unordered_State::get_tiger() const {
    return this->tiger;
}


const set<int>& Unordered_State::rows_in_col(int col) const {
    // set col to nearest column in range for index protection
    col = max(0, col);
    col = min(col, NUM_COL - 1);

    return this->col_to_rows[col];
}


/// Mutator Functions
void Unordered_State::set_tiger(const Token_t& t) {
    this->tiger = t;
}


void Unordered_State::set_men_locs(const set<Point_t>& locs) {
    for (auto &col_to_row : col_to_rows) {
        col_to_row = set<int>();
    }
    for (auto loc : locs) {
        if(loc.col >= 0 && loc.col < NUM_COL) {
            col_to_rows[loc.col].insert(loc.row);
        }
    }
}


/// operators
bool Unordered_State::do_move(const Move_t& m) {
    bool valid_move = true; // true iff moving a token that
    // exists to an unoccupied position
    Point_t from = m.token.location; // where going from
    Point_t to = m.destination; // where going to
    // this is a bad move if not in the column range
    if(from.col < 0 || from.col >= NUM_COL || to.col < 0 ||
       to.col >= NUM_COL) {
        valid_move = false;
    }
    // this is a bad move if moving into tiger location
    if(to == this->tiger.location) {
        valid_move = false;
    }

    // moving a man
    if(valid_move && m.token.color == BLUE) {
        // try to erase previous location, if cannot then trying to move
        // a token which does not exist
        if(this->col_to_rows[from.col].erase(from.row) <= 0) {
            valid_move = false;
        } // if did erase, but moving to an occupied position, that is invalid
        else if(!(this->col_to_rows[to.col].insert(to.row).second)){
            valid_move = false;
            this->col_to_rows[from.col].insert(from.row);
        }
    }// moving a tiger
    else if(valid_move && m.token.color == RED) {
        if(from != this->tiger.location) {
            valid_move = false;
        }
        else if(this->col_to_rows[to.col].find(to.row) ==
                this->col_to_rows[to.col].end()) {
            this->tiger.location = to;
        }
        else {
            valid_move = false;
        }

        // if a jump, remove the man that was jumped
        Point_t jumped_pos = (to + from) / 2;
        bool is_jump = true;
        if(jumped_pos == to || jumped_pos == from) {
            is_jump = false;
        }
        else if(rows_in_col(jumped_pos.col).find(jumped_pos.row) !=
                rows_in_col(jumped_pos.col).end()) {
            is_jump = false;
        }
        if(is_jump) {
            col_to_rows[jumped_pos.col].erase(jumped_pos.row);
        }
    }

    return valid_move;
}


Unordered_State::operator State() const{
    State st;
    st.push_back(this->tiger);
    for(int c = 0; c < NUM_COL; ++c) {
        for(auto r = col_to_rows[c].begin(); r != col_to_rows[c].end(); ++r) {
            st.push_back(make_man(make_point(*r, c)));
        }
    }
    return st;
}


/// functions to convenience getting information
bool Unordered_State::is_occupied(const Point_t& pt) const {
    bool occupied = false;
    // if column in range
    if(pt.col >= 0 && pt.col < NUM_COL) {
        if(pt == this->tiger.location) {
            occupied = true;
        }
        else if(col_to_rows[pt.col].find(pt.row) != col_to_rows[pt.col].end()){
            occupied = true;
        }
    }

    return occupied;
}


//
// Created by iancl on 3/25/2018.
//

Move_t  Move_Deep_Blue(vector<Token_t> gameState, Color_t turn){
    static Smart_Mover * men  = nullptr;
    // if at beginning of game, make a new man
    bool new_game = true;
    for(unsigned int i = 0; i < gameState.size() && new_game; ++i) {
        if(gameState[i].color == BLUE) {
            if(gameState[i].location.row < NUM_ROW - 2) {
                new_game = false;
            }
        }
    }
    if(new_game && men != nullptr) {
        delete men;
        men = nullptr;
    }

    if(men == nullptr || new_game) {
        men = new Smart_Mover(gameState);
    }
    Move_t returnMove;
    GameRunner tigerMove;

    if(turn == RED){
        returnMove = tigerMove.Tiger_Move(gameState, 1);
    }
    else {
        returnMove = men->next_move(gameState);
    }

    return returnMove;
}

/*
 * Default Constructor can be used to create default start game
 */
GameRunner::GameRunner(){
    istringstream graphFile(graph), startingPos(startPos);
    this->createGraph(graphFile, startingPos);
}

/*
 * Custom constructor can make custom game with new map and new starting pos
 */
GameRunner::GameRunner(std::istream & graphFile, std::istream & startingPos){
    this->createGraph(graphFile, startingPos);
}

/*
 * Custom constructor can make custom game with default map custom locations
 */
GameRunner::GameRunner(std::istream & startingPos){
    istringstream graphFile(graph);
    this->createGraph(graphFile, startingPos);
}

void GameRunner::createGraph(istream & graphFile, istream & startingPos){
    this->tiger_cage_row_offset = 4;
    string trashline = "";
    this->gameState = new vector<Token_t>();
    this->extendedGraph = new map<Point_t, list<Point_t> >();
    int readNum;
    Token_t tempToken;
    Point_t tempPoint, tempPoint2;
    list<Point_t> tempList;
    map<Point_t, list<Point_t> >::iterator mapIter, mapIter2;
    //Read in the board
    getline(graphFile, trashline);
    //Read in the square section
    graphFile >> this->row_boundary >> this->col_boundary;
    //Read in the Unusual edges
    graphFile.ignore();
    getline(graphFile, trashline);
    while(graphFile >> readNum){
        graphFile >> tempPoint.row >> tempPoint.col;
        mapIter = (extendedGraph->insert(make_pair(tempPoint,
                                                   tempList))).first;
        for(int i = 0; i < readNum; i++) {
            graphFile >> tempPoint2.row >> tempPoint2.col;
            mapIter->second.push_back(tempPoint2);
            mapIter2 = (extendedGraph->insert(make_pair(tempPoint2,
                                                        tempList))).first;
            mapIter2->second.push_back(tempPoint);
        }
    }

    //Read in the piece locations
    tempToken.color = RED;
    getline(startingPos, trashline);
    startingPos >> tempToken.location.row >> tempToken.location.col;
    this->gameState->push_back(tempToken);
    tempToken.color = BLUE;
    startingPos.ignore();
    getline(startingPos, trashline);
    //Read in the men
    while(startingPos >> tempToken.location.row >> tempToken.location.col){
        this->gameState->push_back(tempToken);
    }
}

bool GameRunner::isValidMove(vector <Token_t> const & moves, Move_t move) {
    this->manJumpedLastCheck = false;
    Point_t jumpedMan;
    if(move.destination.col < 0 || move.destination.row < 0
       || move.destination.col >= col_boundary || move.destination.row >=
                                                  row_boundary)
        return false;

    bool validMove = false, tigerJumpedMan = false;
    int jumpedManCol, jumpedManRow;
    for(unsigned i = 0; i < moves.size(); i++){
        //See if another token occupies that location
        if(moves[i].location == move.destination){
            return false;
        }
        //Checking if move indicated is valid IF token found is right token
        if(moves[i] == move.token && !validMove) {
            // to check for special edges
            map<Point_t, list<Point_t> >::iterator mapIter;

            bool inSquareSection = false;
            int destRow = move.destination.row, destCol = move.destination.col,
                    origRow = moves[i].location.row, origCol =
                    moves[i].location.col;
            int colDifference = destCol - origCol,
                    rowDifference = destRow - origRow;
            colDifference = (colDifference < 0) ?
                            colDifference * -1 : colDifference;
            rowDifference = (rowDifference < 0) ?
                            rowDifference * -1 : rowDifference;
            if(origRow >= tiger_cage_row_offset &&
               origRow < row_boundary && origCol < col_boundary
               && destRow >= tiger_cage_row_offset &&
               destRow < row_boundary && destCol < col_boundary){
                    inSquareSection = true;
            }
            //Men can only move 1 ever except Tiger cage
            if(move.token.color == BLUE){
                if((rowDifference > 1 || colDifference > 1)) {
                    return false;
                }
            }
            if(move.token.color == RED){
                if((rowDifference > 2 || colDifference > 2)) {
                    return false;
                }
                //See if tiger jumped man in square section
                if(rowDifference == 2 || colDifference == 2){
                    tigerJumpedMan = true;
                    //Now find coordinates of jumped dude
                    jumpedMan.col = jumpedManCol =
                            (move.destination.col + origCol) / 2;
                    jumpedMan.row = jumpedManRow =
                            (move.destination.row + origRow) / 2;
                }
            }
            //See if the move starts and ends in Square and is not diagonal
            if((rowDifference > 0 && colDifference == 0)
               || (colDifference > 0 && rowDifference == 0)){
                if(inSquareSection){
                    validMove = true;
                }
            }
                //See if the move involved an unusual edge in some way
            else if((mapIter = extendedGraph->find(moves[i].location))
                    != extendedGraph->end()){
                list<Point_t>::const_iterator listIter =
                        mapIter->second.begin();
                while(!validMove && listIter != mapIter->second.end()){
                    if(move.destination == *listIter){
                        validMove = true;
                    }
                    else if(tigerJumpedMan && jumpedMan == *listIter){
                        map<Point_t, list<Point_t> >::iterator mapIter2;
                        mapIter2 = extendedGraph->find(jumpedMan);
                        list<Point_t>::const_iterator listIter2 =
                                mapIter2->second.begin();
                        while(!validMove && listIter2 !=
                                            mapIter2->second.end()){
                            if(move.destination == *listIter2){
                                validMove = true;
                            }
                            listIter2++;
                        }
                    }
                    listIter++;
                }
                //Move end position not reachable from indicated start position
                if(!validMove){
                    return false;
                }
            }
                //Move start position invalid
            else{
                return false;
            }
        }
    }
    //See if a man was actual present where the tiger jumped him
    if(tigerJumpedMan && validMove){
        validMove = false;
        //See if a man is present at the jumped position
        for(unsigned i = 1 ; i < moves.size() && !validMove; i++){
            if(moves[i].location.row == jumpedManRow &&
               moves[i].location.col == jumpedManCol){
                validMove = true;
            }
        }
        //If the Tiger jump was actually valid store the info for later use
        if(validMove){
            this->manJumpedLastCheck = true;
            this->manJumpedCol = jumpedManCol;
            this->manJumpedRow = jumpedManRow;
        }
    }
    return validMove;
}


bool GameRunner::isValidMove(Unordered_State const & st, Move_t move) {
    this->manJumpedLastCheck = false;
    Point_t jumpedMan;
    Point_t from = move.token.location, to = move.destination;
    // if easily seen to be out of bounds, return false
    if(to.col < 0 || to.row < 0 ||
       to.col >= col_boundary || to.row >= row_boundary) {
        return false;
    }
    // ensure moving an existing token
    bool existing_token = true;
    if(!st.is_occupied(move.token.location)) {
        existing_token = false;
    }
    else if(move.token.color == RED && st.get_tiger() != move.token) {
        existing_token = false;
    }
    else if(move.token.color == BLUE &&
            st.rows_in_col(from.col).find(from.row) ==
            st.rows_in_col(from.col).end()) {
        existing_token = false;
    }
    if(!existing_token) {
        return false;
    }

    bool validMove = false, tigerJumpedMan = false;
    // if the destination is occupied, return false
    if(st.is_occupied(to)) {
        return false;
    }
    //Checking if move indicated is valid
    map<Point_t, list<Point_t> >::iterator mapIter;// to check for special edges
    bool inSquareSection = false;
    Point_t diff = abs(to - from);
    if(from.row >= tiger_cage_row_offset && from.row <
            row_boundary && from.col < col_boundary
       && to.row >= tiger_cage_row_offset && to.row <
               row_boundary && to.col < col_boundary){
        inSquareSection = true;
    }
    //Men can only move 1
    if(move.token.color == BLUE){
        if((diff.row > 1 || diff.col > 1)) {
            return false;
        }
    }
    if(move.token.color == RED){
        if(diff.row > 2 || diff.col > 2) {
            return false;
        }
        //See if tiger jumped man in square section
        if(diff.row == 2 || diff.col == 2){
            tigerJumpedMan = true;
            //Now find coordinates of jumped dude
            jumpedMan = (to + from) / 2;
        }
    }
    //See if the move starts and ends in the Square section and not diagonal
    if((diff.row > 0 && diff.col == 0) || (diff.col > 0
                                           && diff.row == 0)){
        if(inSquareSection){
            validMove = true;
        }
    }
        //See if the move involved an unusual edge in some way
    else if((mapIter = extendedGraph->find(from)) != extendedGraph->end()){
        list<Point_t>::const_iterator listIter = mapIter->second.begin();
        while(!validMove && listIter != mapIter->second.end()){
            if(move.destination == *listIter){
                validMove = true;
            }
            else if(tigerJumpedMan && jumpedMan == *listIter){
                map<Point_t, list<Point_t> >::iterator mapIter2;
                mapIter2 = extendedGraph->find(jumpedMan);
                list<Point_t>::const_iterator listIter2 =
                        mapIter2->second.begin();
                while(!validMove && listIter2 !=
                                    mapIter2->second.end()){
                    if(to == *listIter2){
                        validMove = true;
                    }
                    listIter2++;
                }
            }
            listIter++;
        }
        //Move end position not reachable from the indicated start position
        if(!validMove){
            return false;
        }
    }//Move start position invalid
    else{
        return false;
    }

    //See if a man was actually present where the tiger jumped him
    if(tigerJumpedMan && validMove){
        validMove = false;
        //See if a man is present at the jumped position
        if(st.is_occupied(jumpedMan)) {
            validMove = true;
        }
        //If the Tiger jump was actually valid store the info for later use
        if(validMove){
            this->manJumpedLastCheck = true;
            this->manJumpedCol = jumpedMan.col;
            this->manJumpedRow = jumpedMan.row;
        }
    }
    return validMove;
}


bool GameRunner::evaluateWinState( vector <Token_t> & tokens, Color_t & color){
    bool returnFlag = false;
    pair<Point_t *, pair<bool *, int> > moveReceiver =
            this->validMoves(tokens, tokens[0]);
    //Means the tiger cannot move so MEN WIN
    if(moveReceiver.second.second == 0){
        color = BLUE;
        returnFlag = true;
    }
    //If at least one man alive Tiger has not won yet
    if(!returnFlag && tokens.size() < 1){
        color = RED;
        returnFlag = true;
    }
    if(moveReceiver.second.first) {
        delete[] moveReceiver.second.first;
        moveReceiver.second.first = nullptr;
    }
    if(moveReceiver.first) {
        delete[] moveReceiver.first;
        moveReceiver.first = nullptr;
    }
    return returnFlag;
}

pair<bool, Color_t> GameRunner::playGame(){
    Color_t turn = RED, winner;
    Move_t returnedMove;
    int counter = 0;
    //Keep playing until game is finished or 10000 turns have passed
    while(counter < 10000 && !this->evaluateWinState(*this->gameState,
                                                     winner)){
        this->manJumpedLastCheck = false;
        counter++;
        returnedMove = Move_Deep_Blue(*this->gameState, turn);
        //If not valid move I will perform first available move
        if(!isValidMove(*this->gameState, returnedMove)){
            pair<Point_t *, pair<bool *, int>> tempHolder;
            bool found = false;
            for(unsigned int i = (turn == RED ? 0 : 1);
                i < gameState->size() && !found; i++){
                returnedMove.token = (*gameState)[i];
                pair<Point_t *, pair<bool *, int > > returnPair =
                        this->validMoves(*gameState, returnedMove.token);
                if(returnPair.second.second > 0){
                    returnedMove.destination = returnPair.first[0];
                    found = true;
                }
                delete [] returnPair.first;
                delete [] returnPair.second.first;
            }

        }
        for(vector<Token_t>::iterator vectorIterator = gameState->begin();
            vectorIterator != gameState->end(); vectorIterator++) {
            //See if the encountered spot is the moved token
            if ((*vectorIterator).location == returnedMove.token.location) {
                (*vectorIterator).location = returnedMove.destination;
            }
            //See if a man was killed that turn
            if (turn == RED && this->manJumpedLastCheck) {
                if ((*vectorIterator).location.row == manJumpedRow &&
                    (*vectorIterator).location.col == manJumpedCol) {
                    this->gameState->erase(vectorIterator);
                    break;
                }
            }
        }
        turn = (turn == RED ? RED : BLUE);
    }
    if(counter >= 10000) {
        return make_pair(false, RED);
    }else if(winner == RED){
        return make_pair(true, RED);
    }
    return make_pair(true, BLUE);
}

pair<Point_t *, pair<bool *, int> > GameRunner::validMoves(
        vector <Token_t> const & boardState, Token_t piece){
    //Maximum number of valid moves
    Point_t * validPoints = new Point_t[MAX_NUMBER_MOVES];
    bool * jumpMade = new bool[MAX_NUMBER_MOVES];
    int size = 0;
    Move_t tempMove, jumpMove;
    tempMove.token = piece;
    jumpMove.token = piece;
    //This accounts for basic non diagonal moves
    for(int i = 0; i < 4; i++){
        tempMove.destination = jumpMove.destination = piece.location;
        switch(i) {
            case 0:
                tempMove.destination.row += 1;
                jumpMove.destination.row += 2;
                break;
            case 1:
                tempMove.destination.row -= 1;
                jumpMove.destination.row -= 2;
                break;
            case 2:
                tempMove.destination.col += 1;
                jumpMove.destination.col += 2;
                break;
            case 3:
                tempMove.destination.col -= 1;
                jumpMove.destination.col -= 2;
        }
        //See if moving simply UP,DOWN,LEft,RiGHT 1 is valid
        if(this->isValidMove(boardState, tempMove)){
            validPoints[size] = tempMove.destination;
            jumpMade[size] = false;
            size += 1;
        } //See if moving 2 UP,DOWN,LEFT,RIGHt works
        else if (piece.color == RED && isValidMove(boardState, jumpMove)){
            validPoints[size] = jumpMove.destination;
            jumpMade[size] = true;
            size += 1;
        }
    }
    //Now check for diagonal moves
    map<Point_t, list<Point_t> >::const_iterator mapIter =
            extendedGraph->find(piece.location);
    list<Point_t>::const_iterator listIter;
    if(mapIter != extendedGraph->end()){
        listIter = mapIter->second.begin();
        while(listIter != mapIter->second.end()){
            tempMove.destination = *listIter;
            jumpMove.destination.row = ((tempMove.destination.row -
                                         piece.location.row) * 2) +
                                       piece.location.row;
            jumpMove.destination.col = ((tempMove.destination.col -
                                         piece.location.col) * 2) +
                                       piece.location.col;
            //See if diagonal move valid
            if(isValidMove(boardState, tempMove)) {
                validPoints[size] = tempMove.destination;
                jumpMade[size] = false;
                size += 1;
            }//See if man can be jumped with diagonal
            else if(piece.color == RED && isValidMove(boardState, jumpMove)){
                validPoints[size] = jumpMove.destination;
                jumpMade[size] = true;
                size += 1;
            }
            listIter++;
        }
    }
    //return the pointer coupled with the number of elements stored in it
    return make_pair(validPoints, make_pair(jumpMade, size));
}


pair<Point_t *, pair<bool *, int> > GameRunner::validMoves(
        Unordered_State const & boardState, Token_t piece){
    //Maximum number of valid moves
    Point_t * validPoints = new Point_t[MAX_NUMBER_MOVES];
    bool * jumpMade = new bool[MAX_NUMBER_MOVES];
    int size = 0;
    Move_t tempMove, jumpMove;
    //This accounts for basic non diagonal moves
    for(int i = 0; i < 4; i++){
        Point_t dir;
        switch(i) {
            case 0:
                dir = UP;
                break;
            case 1:
                dir = DOWN;
                break;
            case 2:
                dir = RIGHT;
                break;
            case 3:
                dir = LEFT;
        }
        tempMove = make_move_in_direction(piece, dir);
        jumpMove = make_move_in_direction(piece, 2 * dir);
        //See if moving simply UP,DOWN,LEft,RiGHT 1 is valid
        if(this->isValidMove(boardState, tempMove)){
            validPoints[size] = tempMove.destination;
            jumpMade[size] = false;
            size += 1;
        } //See if moving 2 UP,DOWN,LEFT,RIGHt works
        else if (piece.color == RED && isValidMove(boardState, jumpMove)){
            validPoints[size] = jumpMove.destination;
            jumpMade[size] = true;
            size += 1;
        }
    }
    //Now check for diagonal moves
    tempMove.token = jumpMove.token = piece; // ensure token is piece
    map<Point_t, list<Point_t> >::const_iterator mapIter =
            extendedGraph->find(piece.location);
    list<Point_t>::const_iterator listIter;
    if(mapIter != extendedGraph->end()){
        listIter = mapIter->second.begin();
        while(listIter != mapIter->second.end()){
            tempMove.destination = *listIter;
            jumpMove.destination = tempMove.destination +
                                   (tempMove.destination -
                                    tempMove.token.location);
            //See if diagonal move valid
            if(isValidMove(boardState, tempMove)) {
                validPoints[size] = tempMove.destination;
                jumpMade[size] = false;
                size += 1;
            }//See if man can be jumped with diagonal
            else if(piece.color == RED && isValidMove(boardState, jumpMove)){
                validPoints[size] = jumpMove.destination;
                jumpMade[size] = true;
                size += 1;
            }
            listIter++;
        }
    }
    //return the pointer coupled with the number of elements stored in it
    return make_pair(validPoints, make_pair(jumpMade, size));
}


Move_t GameRunner::Tiger_Move(vector<Token_t> & tokens, int randomProbability){
    Move_t returnMove;
    returnMove.token.location = tokens[0].location;
    returnMove.token.color = RED;

    srand(time(NULL));
    //Get all of the valid moves for the Tiger
    pair<Point_t *, pair<bool *, int> > returnMoves =
            this->validMoves(tokens, tokens[0]);
    set<Point_t> checkPoints;
    pair<Point_t * , pair< bool *, int > > tokenMoves;
    static Point_t previousLocation;
    Point_t closestPoint, record = NULL_POINT, record2;
    int smallestRowColDistance = 1000, rowDifference, colDifference,
            destRow,destCol, origRow, origCol, totaler;

    bool moveFound = false;
    origCol = tokens[0].location.col;
    origRow = tokens[0].location.row;

    //First see if jump can be made
    //Will probably implement min max here later
    for(int i = 0; i < returnMoves.second.second; i++){
        //Greedily selects a jump
        if(returnMoves.second.first[i]){
            returnMove.destination = returnMoves.first[i];
            moveFound = true;
        }
    }
    if(!moveFound){
        //Act randomly
        if(9 -(rand() % 10) < randomProbability && returnMoves.second.second > 0){
            //If random make random move
            returnMove.destination = returnMoves.first[rand() %
                                                       returnMoves.second.second];
            if(returnMoves.second.first) {
                delete[] returnMoves.second.first;
                returnMoves.second.first = nullptr;
            }
            if(returnMoves.first) {
                delete[] returnMoves.first;
                returnMoves.first = nullptr;
            }
            return returnMove;
        }
        //Temporarily reset tiger Position
        record2 = tokens[0].location;
        tokens[0].location = record;
        //Add all points within 1 of the Men to the set to be evaluated
        for(unsigned int i = 1; i < tokens.size(); i++){
            tokenMoves = validMoves(tokens, tokens[i]);
            for(int j = 0; j < tokenMoves.second.second;j++){
                checkPoints.insert(tokenMoves.first[j]);
            }
            if(tokenMoves.second.first) {
                delete[] tokenMoves.second.first;
                tokenMoves.second.first = nullptr;
            }
            if(tokenMoves.first) {
                delete[] tokenMoves.first;
                tokenMoves.first = nullptr;
            }
        }
        tokens[0].location = record2;
        //Now find the closest point
        for(Point_t toCheck : checkPoints){
            destRow = toCheck.row;
            destCol = toCheck.col;
            colDifference = destCol - origCol;
            rowDifference = destRow - origRow;
            colDifference = (colDifference < 0) ? colDifference * -1 :
                            colDifference;
            rowDifference = (rowDifference < 0) ? rowDifference * -1 :
                            rowDifference;
            totaler = rowDifference + colDifference;
            //see if it is a point that is more desirable i.e. has diagonal
            //We want tiger to gravitate to diagonal edges
            if(this->extendedGraph->find(toCheck) != extendedGraph->end()){
                totaler -= 3;
            }
            if(totaler < smallestRowColDistance){
                smallestRowColDistance = totaler;
                closestPoint = toCheck;
            }
        }
        //See if tiger is already on the closest point
        bool alreadyClosest = (closestPoint == tokens[0].location);
        //See if should alternate if already there
        if(alreadyClosest) {
            map<Point_t, list<Point_t>>::const_iterator mapIter =
                    this->extendedGraph->find(tokens[0].location);
            //See if non traditional move can be made because they
            // are harder to counter
            if (mapIter != extendedGraph->end()) {
                for (Point_t temp : mapIter->second) {
                    returnMove.destination = temp;
                    if (isValidMove(tokens, returnMove)) {
                        moveFound = true;
                        break;
                    }
                }
            }
            //If no non traditional move can be found see if can just move
            // back to previous location
            if (!moveFound) {
                //Try and go upwards
                for(int k = 0; !moveFound && k < returnMoves.second.second;
                    k++) {
                    returnMove.destination = returnMoves.first[k];
                    if((returnMove.destination.row - origRow != 0)) {
                        moveFound = true;
                    }
                }
                if(!moveFound){
                    moveFound = true;
                    record = returnMove.destination;
                    returnMove.destination = previousLocation;
                    if (!isValidMove(tokens, returnMove))
                        returnMove.destination = record;
                }
            }
        }
            //Case not already as close as can be then I will BFS to him
        else{
            bool success;
            returnMove.destination = BFS_To_Point(tokens, 0, closestPoint,
                                                  RED, success);
            //If no path can be found then just make a move
            if(!success){
                returnMove.destination = returnMoves.first[0];
            }
        }
    }
    if(returnMoves.second.first) {
        delete[] returnMoves.second.first;
        returnMoves.second.first = nullptr;
    }
    if(returnMoves.first) {
        delete[] returnMoves.first;
        returnMoves.first = nullptr;
    }
    previousLocation = tokens[0].location;
    return  returnMove;
}

Point_t GameRunner::BFS_To_Point(vector<Token_t> mapLayout,
                                 int tokenIndex, Point_t desiredLoc,
                                 Color_t color, bool & success){
    queue<Point_t> frontier;
    map<Point_t, Point_t> previous;
    Point_t evaluatePoint, originalPoint = mapLayout[tokenIndex].location,
            temp, temp2;
    Token_t currToken;
    currToken.color = color;
    frontier.push(mapLayout[tokenIndex].location);
    previous[currToken.location] = currToken.location;
    pair<Point_t * , pair< bool *, int > > tokenMoves;
    while(!frontier.empty()){
        //Reseting the position because validMoves will check if a
        // piece actually exists there!
        //Okay to change layout because vector is a copy :-)
        currToken.location = mapLayout[tokenIndex].location = frontier.front();
        frontier.pop();
        //See if done
        if(currToken.location == desiredLoc){
            break;
        }

        tokenMoves = this->validMoves(mapLayout, currToken);
        for(int i = 0; i < tokenMoves.second.second; i++){
            evaluatePoint = tokenMoves.first[i];
            //See if the key has a predecessor i.e. exists if not add
            if(previous.insert(make_pair(evaluatePoint,
                                         currToken.location)).second) {
                frontier.push(evaluatePoint);
            }
        }
        if(tokenMoves.second.first) {
            delete[] tokenMoves.second.first;
            tokenMoves.second.first = nullptr;
        }
        if(tokenMoves.first) {
            delete[] tokenMoves.first;
            tokenMoves.first = nullptr;
        }
    }
    if(currToken.location == desiredLoc) {
        //Now find what Point should be moved to
        temp = evaluatePoint = currToken.location;;
        while(!(temp == originalPoint)){
            evaluatePoint = temp;
            temp2 = previous.find(temp)->second;
            temp = temp2;
        }
        success = true;
    }
    else{
        success = false;
    }
    return evaluatePoint;

}

int GameRunner::getRandomizer(vector<Token_t> & tokens){
    //First find the average row
    int rowTotal = 0, rowAverage;
    for(Token_t & temp : tokens){
        rowTotal += temp.location.row;
    }
    rowAverage = rowTotal / tokens.size();
    //Now just return a higher randomizer based on stage of play
    if(rowAverage <= 8)
        return 2;
    if(rowAverage <= 14)
        return 1;
    return 0;
}

Move_t GameRunner::Undeterministic_Tiger_Move(vector<Token_t> & pieces){
    static list<Point_t> previousPoints;
    set<Point_t> movedPoints(previousPoints.begin(), previousPoints.end());
    Move_t returnMove;
    returnMove = (movedPoints.size() == 2 ? this->Tiger_Move(pieces, 0) :
                  this->Tiger_Move(pieces, this->getRandomizer(pieces)));
    previousPoints.push_back(returnMove.destination);
    if(previousPoints.size() > 6)
        previousPoints.pop_front();
    return returnMove;
}

/**********************************            ********************************
*********************************** Point Util ********************************
***********************************            *******************************/

/// Point constructors
Point_t make_point(int row, int col) {
    Point_t pt;
    pt.row = row;
    pt.col = col;
    return pt;
}

Point_t make_point(const pair<int,int>& p) {
    Point_t pt;
    pt.row = p.first;
    pt.col = p.second;
    return pt;
}


/// Point comparators
bool operator<(const Point_t& a, const Point_t& b) {
    bool lessThan = false;
    if(a.row < b.row) {
        lessThan = true;
    }
    else if(a.row == b.row && a.col < b.col) {
        lessThan = true;
    }
    return lessThan;
}


bool operator>(const Point_t& a, const Point_t& b) {
    bool greaterThan = false;
    if(a.row > b.row) {
        greaterThan = true;
    }
    else if(a.row == b.row && a.col > b.col) {
        greaterThan = true;
    }
    return greaterThan;
}


bool operator<=(const Point_t& a, const Point_t& b) {
    bool lessThanOrEqual = false;
    if(a.row < b.row) {
        lessThanOrEqual = true;
    }
    else if(a.row == b.row && a.col <= b.row) {
        lessThanOrEqual = true;
    }
    return lessThanOrEqual;
}


bool operator>=(const Point_t& a, const Point_t &b) {
    bool greaterThanOrEqual = false;
    if(a.row > b.row) {
        greaterThanOrEqual = true;
    }
    else if(a.row == b.row && a.col >= b.col) {
        greaterThanOrEqual = true;
    }
    return greaterThanOrEqual;
}


bool operator==(const Point_t& a, const Point_t& b) {
    return (a.row == b.row && a.col == b.col);
}


bool operator!=(const Point_t& a, const Point_t& b) {
    return (a.row != b.row || a.col != b.col);
}


/// Point operators
Point_t operator+(const Point_t& a, const Point_t& b) {
    Point_t pt(a); // copy of a
    pt += b;
    return pt;
}

void operator+=(Point_t& a, const Point_t& b) {
    a.row += b.row;
    a.col += b.col;
}

Point_t operator-(const Point_t& a, const Point_t& b) {
    Point_t pt(a); // copy of a
    pt -= b;
    return pt;
}

void operator-=(Point_t& a, const Point_t& b) {
    a.row -= b.row;
    a.col -= b.col;
}

Point_t operator*(int scalar, const Point_t& vect) {
    Point_t pt = vect;
    pt.row *= scalar;
    pt.col *= scalar;
    return pt;
}

Point_t operator/(const Point_t& vect, int scalar) {
    Point_t pt = vect;
    pt.row /= scalar;
    pt.col /= scalar;
    return pt;
}

Point_t abs(const Point_t& a) {
    return make_point(abs(a.row), abs(a.col));
}


int one_norm(const Point_t& a) {
    return abs(a.row) + abs(a.col);
}


/**********************************            ********************************
*********************************** Token Util ********************************
***********************************            *******************************/

/// Token Constructors

Token_t make_token(Color_t c, const Point_t& pt) {
    Token_t tn;
    tn.color = c;
    tn.location = pt;
    return tn;
}


Token_t make_man(const Point_t& pt) {
    Token_t tn;
    tn.color = BLUE;
    tn.location = pt;
    return tn;
}


Token_t make_tiger(const Point_t& pt) {
    Token_t tn;
    tn.color = RED;
    tn.location = pt;
    return tn;
}


/// Token Comparators
bool operator<(const Token_t& a, const Token_t& b) {
    return (a.location < b.location);
}


bool operator>(const Token_t& a, const Token_t& b) {
    return (a.location > b.location);
}


bool operator<=(const Token_t& a, const Token_t& b) {
    return (a.location < b.location) || (a == b);
}


bool operator>=(const Token_t& a, const Token_t &b) {
    return (a.location > b.location) || (a == b);
}


bool operator==(const Token_t& a, const Token_t& b) {
    return (a.location == b.location) && (a.color == b.color);
}


bool operator!=(const Token_t& a, const Token_t& b) {
    return (a.location != b.location) || (a.color != b.color);
}


/**********************************            ********************************
*********************************** Move Util  ********************************
***********************************            *******************************/

/// Move Constructors
Move_t make_move(const Token_t& tn, const Point_t& dest) {
    Move_t mv;
    mv.token = tn;
    mv.destination = dest;
    return mv;
}


Move_t make_move_in_direction(const Token_t& tn, const Point_t& direc) {
    Move_t mv;
    mv.token = tn;
    mv.destination = tn.location + direc;
    return mv;
}

/// Move comparators
bool operator==(const Move_t& a, const Move_t& b){
    return a.token == b.token && a.destination == b.destination;
}


bool operator!=(const Move_t &a, const Move_t& b) {
    return !(a == b);
}


bool operator<(const Move_t& a, const Move_t& b) {
    return (a.token != b.token ? a.token < b.token : a.destination <
            b.destination);
}


/// Move operators
Move_t operator-(const Move_t& m) {
    Move_t mv = m;
    swap(mv.token.location, mv.destination);
    return mv;
}


void operator+=(Move_t &a, const Move_t& b) {
    a.destination += b.destination;
    a.token.location += b.token.location;
}


Move_t operator+(const Move_t& a, const Move_t& b) {
    Move_t m = a;
    m += b;
    return m;
}


int one_norm(const Move_t & m) {
    return one_norm(m.destination - m.token.location);
}

Men_Mover::Men_Mover(const State &s) : current(s) {}

Move_t Men_Mover::next_move(State s) {
    this->current = Unordered_State(s);
    return execute_move();
}


Hash_val hash_locs(const Unordered_State& st, int back_row) {
    Hash_val h = make_pair(0,0);
    // for each man
    for(int c = 0; c < NUM_COL; ++c) {
        for(auto r = st.rows_in_col(c).begin(); r !=
                                                st.rows_in_col(c).end(); ++r) {
            Point_t loc = make_point(*r, c);
            int val = pow(2, loc.col + ((back_row - loc.row) % 2)
                                       * NUM_COL);

            if(back_row - loc.row < 2) {
                h.first += val;
            }
            else {
                h.second += val;
            }
        }
    }


    return h;
}


Hash_val next_hash(const Move_t& m, Hash_val old_hash, int back_row) {
    // assert not moving tiger
    Hash_val h = old_hash;
    Point_t from = m.token.location;
    Point_t to = m.destination;

    int from_val, to_val;
    from_val = pow(2, from.col + ((back_row - from.row) % 2) * NUM_COL);
    to_val = pow(2, to.col + ((back_row - to.row) % 2) * NUM_COL);

    // undo from
    if(back_row - from.row < 2) {
        h.first -= from_val;
    }
    else {
        h.second -= from_val;
    }

    // add to
    if(back_row - to.row < 2) {
        h.first += to_val;
    }
    else {
        h.second += to_val;
    }

    return h;
}


bool tiger_can_jump(const Unordered_State* st, GameRunner* g) {
    auto t_moves = g->validMoves(*st, st->get_tiger());

    bool jump = false;
    for(int t = 0; t < t_moves.second.second && !jump; ++t) {
        if(t_moves.second.first[t]){
            jump = true;
        }
    }
    if(t_moves.first) {
        delete[] t_moves.first;
        t_moves.first = nullptr;
    }
    if(t_moves.second.first) {
        delete[] t_moves.second.first;
        t_moves.second.first = nullptr;
    }
    return jump;
}


bool secure(Unordered_State* st, GameRunner* g, Move_t off_move) {
    Token_t orig_tiger = st->get_tiger();
    st->set_tiger(make_tiger(TIGER_START)); // set tiger to starting pt
    // undo off move
    bool off_move_undone = st->do_move(-off_move);

    queue<Point_t> frontier;
    bool jump = false;
    bool visited[NUM_ROW][NUM_COL];
    for(int r = 0; r < NUM_ROW; ++r) {
        for(int c = 0; c < NUM_COL; ++c) {
            visited[r][c] = false;
        }
    }

    Point_t current = st->get_tiger().location;
    visited[current.row][current.col] = true;
    frontier.push(current);
    while(!frontier.empty() && !jump) {
        current = frontier.front();
        // mark visited
        st->set_tiger(make_tiger(current));
        frontier.pop();

        // if tiger can jump, then this is not secure
        jump = tiger_can_jump(st, g);

        // else, add each reachable new position to the frontier
        if(!jump) {
            auto t_moves = g->validMoves(*st, make_tiger(current));
            Point_t to;
            for(int t = 0; t < t_moves.second.second; ++t) {
                to = t_moves.first[t];
                if(!visited[to.row][to.col]) {
                    visited[to.row][to.col] = true;
                    frontier.push(to);
                }
            }
            // free memory
            if(t_moves.first) {
                delete[] t_moves.first;
                t_moves.first = nullptr;
            }
            if(t_moves.second.first) {
                delete[] t_moves.second.first;
                t_moves.second.first = nullptr;
            }
        }
    }

    // ensure tiger is returned to original pos
    st->set_tiger(orig_tiger);
    // re-do off move
    if(off_move_undone) {
        st->do_move(off_move);
    }

    return !jump;
}


void Smart_Mover::determine_rows() {
    set<int> row_to_col[NUM_ROW];

    /// back_row is the furthest back token
    this->back_row = -1;
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current.rows_in_col(c).begin();
        while(r != current.rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            back_row = max(back_row, *r);
            ++r;
        }
    }

    /// if all tokens on back_row or back_row - 1, front_row
    /// is back_row - 2 (the next row to move to)
    if(row_to_col[back_row].size() == NUM_COL &&
       row_to_col[back_row - 1].size() == NUM_COL) {
        this->front_row = back_row - 2;
    }
        /// else if all columns on back_row - 1 and back_row - 2 except
        /// for one column which is on back_row - 1 and back_row, and tiger
        /// is blocking that column from moving up
        /// front_row is back_row - 3
    else if (row_to_col[back_row - 1].size() == NUM_COL &&
             row_to_col[back_row - 2].size() == NUM_COL - 1 &&
             row_to_col[back_row].size() == 1) {
        int col = *row_to_col[back_row].begin();
        if(row_to_col[back_row - 2].find(col) ==
           row_to_col[back_row - 2].end()){
            if(this->current.get_tiger().location.row >= back_row - 3 &&
               this->current.get_tiger().location.col == col) {
                // if lagging column is
                // - on middle if in lower half
                // - not on an edge if on upper half
                if((front_row >= 8 && col == (NUM_COL - 1) / 2) ||
                   (front_row < 8 && (col == 0 || col == NUM_COL - 1))) {
                    this->front_row = back_row - 3;
                }
            }
        }
    }

    /// otherwise front_row is left unchanged
}


Move_t Smart_Mover::off_move_handling() {
    // ensure that if an off move is purported to exist, it actually does
    // and it can be undone
    if(off_move != NULL_MOVE && (current.get_tiger().location ==
                                 off_move.destination
                                 || current.is_occupied(
                                         off_move.token.location) ||
                                 !current.is_occupied(
                                         off_move.destination))) {
        off_move = NULL_MOVE;
        off_move_active = false;
    }

    off_move_ready = false;
    /// if token in danger because of off move, undo off move
    // or if an off move too far bback, undo it
    Move_t off_move_capture = make_move(current.get_tiger(),
                                        off_move.token.location);
    if((off_move_active && off_move != NULL_MOVE &&
        game.isValidMove(current, off_move_capture))) {
        off_move_active = false;
    }

    Move_t to_do = NULL_MOVE;
    /// if off_move_active but off_move is NULL_MOVE, set an off move
    if(off_move_active && off_move == NULL_MOVE) {
        int off_col;
        // pick column furthest from tiger
        off_col = 0;
        if (current.get_tiger().location.col < (NUM_COL - 1) / 2) {
            off_col = NUM_COL - 1;
        }
        // if not to tiger cage, finish creating off move
        int row = -1;
        // pick lowest token in off_col
        auto r = current.rows_in_col(off_col).begin();
        while (r != current.rows_in_col(off_col).end()) {
            row = max(*r, row);
            ++r;
        }

        // create off move and set to_do to it
        Token_t man = make_man(make_point(row, off_col));
        off_move = make_move_in_direction(man, DOWN);
        // store the off move to-do and set off_move_ready to true
        to_do = off_move;
        off_move_ready = true;
    }
        /// else if not off_move_active and off_move is not NULL_MOVE, undo
        /// the off_move
    else if(!off_move_active && off_move != NULL_MOVE) {
        to_do = -off_move;
        off_move = NULL_MOVE;
        off_move_ready = true;
    }

    /// if doing an off move, cancel all other plans
    if(off_move_ready) {
        move_ready = false;
        path.clear();
        desired.clear();
        this->special_moves.clear_moves();
    }

    return to_do;
}


Move_t** Smart_Mover::find_moves_to_do() {
    /// store forward moves for each column except for the
    /// off_move column, and if there are more than two
    /// men in any column return nullptr
    Move_t** moves_to_do = new Move_t*[NUM_COL];
    for(int c = 0; c < NUM_COL; ++c) {
        moves_to_do[c] = new Move_t[2];
        moves_to_do[c][0] = moves_to_do[c][1] = NULL_MOVE;
        if(c != off_move.destination.col) {
            auto r = current.rows_in_col(c).begin();
            while(r != current.rows_in_col(c).end()) {
                Token_t man = make_man(make_point(*r, c));
                if(moves_to_do[c][0] != NULL_MOVE &&
                   moves_to_do[c][1] != NULL_MOVE) {
                    for(int i = 0; i < NUM_COL; ++i) {
                        if(moves_to_do[i]) {
                            delete[] moves_to_do[i];
                            moves_to_do[i] = nullptr;
                        }
                    }
                    if(moves_to_do) {
                        delete[] moves_to_do;
                        moves_to_do = nullptr;
                    }
                    return nullptr;
                }
                else if(moves_to_do[c][0] == NULL_MOVE) {
                    moves_to_do[c][0] = make_move_in_direction(man, UP);
                }
                else {
                    moves_to_do[c][1] = make_move_in_direction(man, UP);
                    // ensure the first move to do in this column is the token
                    // in front
                    if(moves_to_do[c][0].destination >
                       moves_to_do[c][1].destination){
                        Move_t temp = moves_to_do[c][0];
                        moves_to_do[c][0] = moves_to_do[c][1];
                        moves_to_do[c][1] = temp;
                    }
                    // if the guy in front already has a space between
                    // him and the guy below, don't try to move top guy again
                    if(moves_to_do[c][1].destination.row -
                       moves_to_do[c][0].destination.row > 1) {
                        moves_to_do[c][0] = moves_to_do[c][1];
                        moves_to_do[c][1] = NULL_MOVE;
                    }

                }
                ++r;
            }
        }
    }

    /// ensure that not moving past front line
    for(int c = 0; c < NUM_COL; ++c) {
        if(moves_to_do[c][0].destination.row < front_row) {
            moves_to_do[c][0] = NULL_MOVE;
            if(moves_to_do[c][1].destination.row < front_row + 1) {
                moves_to_do[c][1] = NULL_MOVE;
            }
        }
    }

    return moves_to_do;
}


Move_t Smart_Mover::try_for_specific_move() {
    Move_t to_do = NULL_MOVE;
    special_moves.set_front_back_row(front_row, back_row);

    // if no move ready, then try a special move
    if(special_moves.handle_special_case(off_move)) {
        to_do = special_moves.get_move();
        if(to_do != NULL_MOVE) {
            path.clear();
            desired.clear();
            move_ready = true;
        }
    }

    return to_do;
}


void Smart_Mover::search_for_state() {

    Move_t** moves_to_do = find_moves_to_do();
    if(moves_to_do == nullptr) {
        return;
    }

    move_ready = false;
    int off_col = off_move.destination.col;
    /// look for moves which move forward tokens up to two adjacent columns
    /// from a set of 3
    for(int c = 0; c < NUM_COL - 2 && !move_ready; ++c) {
        if(c == off_col || c + 1 == off_col || c + 2 == off_col) {
            continue;
        }
        desired.clear();
        path.clear();
        // desired states are those which move up either 1 or
        // 2 of the 3 columns
        int ndx = 0;
        int req_moves[6] = {0,0,0,0,0,0};
        for(int i = 0; i < 3; ++i) {
            for(int j = i; j < 3; ++j) {
                Move_t mv;
                Hash_val desired_hash = hash_locs(current, back_row);
                for(int col = 0; col < 3; ++col) {
                    for(int k = 0; k < 2; ++k) {
                        if(col != i && col != j) {
                            mv = moves_to_do[c + col][k];
                            if(mv != NULL_MOVE) {
                                ++req_moves[ndx];
                                current.do_move(mv);
                                desired_hash = next_hash(mv, desired_hash,
                                                         back_row);
                            }
                        }
                    }
                }
                // if the suggested desired state is secure, add it to the set
                // of desired states
                if(secure(&current, &game, off_move) && req_moves[ndx] > 0) {
                    desired.insert(desired_hash);
                }
                else {
                    req_moves[ndx] = 0;
                }
                ++ndx;

                // undo moves
                for(int col = 2; col >= 0; --col) {
                    for(int k = 1; k >= 0; --k) {
                        if(col!= i && col != j) {
                            mv = moves_to_do[c + col][k];
                            if(mv != NULL_MOVE) {
                                current.do_move(-mv);
                            }
                        }
                    }
                }
            }
        }

        // create transition, and have it look for a path to the desired states
        if(!desired.empty()) {
            Transition t(&current, &desired, &game, back_row, make_pair(c, c+3),
                         make_pair(front_row, back_row + 1));
            int min_req = INFTY;
            int max_req = -1;
            for (int req_move : req_moves) {
                max_req = max(max_req, req_move);
                if(req_move > 0) {
                    min_req = min(req_move, min_req);
                }
            }
            move_ready = t.find_path_to_state(
                    min(MAX_RECURSION_DEPTH, max_req), min_req);
            if(move_ready) {
                path = t.get_path();
            }
        }
    }

    // free memory
    for(int c = 0; c < NUM_COL; ++c) {
        if(moves_to_do[c]) {
            delete[] moves_to_do[c];
            moves_to_do[c] = nullptr;
        }
    }
    if(moves_to_do) {
        delete[] moves_to_do;
        moves_to_do = nullptr;
    }
}


Move_t Smart_Mover::fail_safe(Move_t suggested) {
    // first call safety failsafe
    Move_t to_do = safety_fail_safe(suggested);
    if(to_do != NULL_MOVE) {
        suggested = to_do;
    }

    // now ensure validity
    if(!game.isValidMove(current, suggested)) {
        State cur = current;
        // if move not valid, look for a valid safe move
        bool fail_safe_found = false;
        suggested = NULL_MOVE;
        to_do = safety_fail_safe(suggested);
        if(to_do != NULL_MOVE) {
            fail_safe_found = true;
        }
        // try each direction of moves and see if finds one that is valid
        // with no immediage jump
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        // if cannot find a safe valid move, just try to find a valid move
        for (int i = 0; i < 8 && !fail_safe_found; ++i) {
            for (int j = 1; j < (int)cur.size() && !fail_safe_found; ++j) {
                Token_t man = cur[j];
                // don't try to move away from the middle
                if((man.location.col < (NUM_COL - 1) / 2 && dirs[i].col < 0) ||
                    (man.location.col > (NUM_COL - 1) / 2 && dirs[i].col > 0)){
                    continue;
                }
                Move_t mv = make_move_in_direction(man, dirs[i]);
                if (game.isValidMove(current, mv) &&
                    (mv.destination.row >= front_row || to_tiger_cage)) {
                    fail_safe_found = true;
                    to_do = mv;
                }
            }
        }
    }

    return to_do;
}


Move_t Smart_Mover::safety_fail_safe(Move_t suggested) {
    Move_t to_do = NULL_MOVE;

    bool did_move = false;
    if(game.isValidMove(current, suggested)) {
        current.do_move(suggested);
        did_move = true;
    }
    bool can_jump = tiger_can_jump(&current, &game);
    if(did_move) {
        current.do_move(-suggested);
    }

    if(can_jump || !did_move) {
        State cur = current;
        bool fail_safe_found = false;
        // try each direction of moves and see if can prevent the
        // capture
        Point_t dirs[8] = {UP, UP + LEFT, UP + RIGHT,
                           LEFT, RIGHT,
                           DOWN, DOWN + LEFT, DOWN + RIGHT};
        for (int i = 0; i < 8 && !fail_safe_found; ++i) {
            for (int j = 1; j < (int)cur.size() && !fail_safe_found; ++j) {
                Token_t man = cur[j];
                // don't try to move away from the middle if tiger in cage
                bool bad_choice = false;
                if (current.get_tiger().location.row < CAGE_ENTRANCE.row &&
                  ((man.location.col < (NUM_COL - 1) / 2 && dirs[i].col < 0) ||
                   (man.location.col > (NUM_COL - 1) / 2 && dirs[i].col > 0))){
                    bad_choice = true;
                }
                // try not to move off man
                if(man.location == off_move.destination) {
                    bad_choice = true;
                }
                // if at tiger cage, don't move any man in a STAGE_POSITION
                if(to_tiger_cage) {
                    for (auto stage_pos : STAGE_POSITIONS) {
                        if(man.location == stage_pos) {
                            bad_choice = true;
                        }
                    }
                }
                if(bad_choice) {
                    continue;
                }

                Move_t mv = make_move_in_direction(man, dirs[i]);
                if (game.isValidMove(current, mv) &&
                    (to_tiger_cage || mv.destination.row >= front_row)) {
                    current.do_move(mv);
                    if (!tiger_can_jump(&current, &game)) {
                        fail_safe_found = true;
                        to_do = mv;
                    }
                    current.do_move(-mv);
                }
            }
        }
    }


    return to_do;
}


Move_t Smart_Mover::execute_move() {
    Move_t to_do = NULL_MOVE; // men move to do

    Hash_val cur_hash = hash_locs(current, back_row);
    pair<Hash_val, Token_t> ndx = make_pair(cur_hash, current.get_tiger());
    // try a search move
    bool move_from_search = false;
    // see if move still ready from previous search
    if(move_ready) {
        auto next_move = path.find(ndx);
        if(next_move == path.end()) {
            move_ready = false;
        }
        else {
            move_from_search = true;
        }
    }

    // see if move still ready from previous specific move
    if(!move_from_search) {
        to_do = special_moves.get_move();
        if(to_do != NULL_MOVE) {
            move_ready = true;
            path.clear();
            desired.clear();
        }
    }

    if(!move_ready) {
        // determine rows
        determine_rows();
        if(front_row <= 4) {
            to_tiger_cage = true;
        }
            // look for path
        else {
            this->path.clear();
            this->desired.clear();
            this->search_for_state();
            if(move_ready) {
                move_from_search = true;
            }
        }
    }

    // if move is ready from search, do it
    if(move_ready && !to_tiger_cage && move_from_search) {
        cur_hash = hash_locs(current, back_row);
        ndx = make_pair(cur_hash, current.get_tiger());
        to_do = path[ndx];
    }
        // if no move ready from the greedy move, try a special move
    else if(!move_ready){
        to_do = try_for_specific_move();
        if(to_do != NULL_MOVE) {
            move_ready = true;
        }
    }


    // if to tiger cage, try a tiger cage move
    if(to_tiger_cage) {
        ///to_do = kill_tiger_handling();
        to_do = finish_off_tiger();
        if (to_do != NULL_MOVE) {
            move_ready = true;
        }
    }

    // if no move found still, try an off move
    if(!move_ready && !to_tiger_cage){
        off_move_active = !off_move_active;
    }
    // if to tiger cage, turn off the off move
    if(to_tiger_cage) {
        off_move_active = false;
    }

    // handle off move
    Move_t off_move_to_do = off_move_handling();
    if(off_move_ready) {
        to_do = off_move_to_do;
    }


    // perform failsafe
    Move_t fs = fail_safe(to_do);
    if(fs != NULL_MOVE) {
        // if was performing an off move, undo that
        if(off_move_ready) {
            if (off_move_active) {
                off_move_active = false;
                off_move = NULL_MOVE;
            } else {
                off_move_active = true;
                off_move = -to_do;
            }
        }
        // if moving the off move token, turn off move active to false
        if(fs.token.location == off_move.destination) {
            off_move = NULL_MOVE;
            off_move_active = false;
        }
        move_ready = false;
        this->path.clear();
        this->desired.clear();
        this->special_moves.clear_moves();
        to_do = fs;
    }

    return to_do;
}


set<Point_t> Smart_Mover::tiger_reachable_pos() {
    // determine positions reachable by tiger using a bfs
    set<Point_t> tiger_reachable;
    queue<Point_t> frontier;
    frontier.push(current.get_tiger().location);
    tiger_reachable.insert(current.get_tiger().location);
    Point_t orig_tiger_pos = current.get_tiger().location;
    while(!frontier.empty()) {
        Point_t tig_pos = frontier.front();
        frontier.pop();
        current.set_tiger(make_tiger(tig_pos));
        auto t_moves = game.validMoves(current, current.get_tiger());
        // for each position reachable from tig_pos, mark as reachable
        // and if not been there before, add to the frontier
        for(int i = 0; i < t_moves.second.second; ++i) {
            Point_t dest = t_moves.first[i];
            if(tiger_reachable.insert(dest).second) {
                frontier.push(dest);
            }
        }
        // free memory
        if(t_moves.first) {
            delete [] t_moves.first;
            t_moves.first = nullptr;
        }
        if(t_moves.second.first) {
            delete [] t_moves.second.first;
            t_moves.second.first = nullptr;
        }
    }
    // return tiger to original position
    current.set_tiger(make_tiger(orig_tiger_pos));
    // remove current tiger position from reachable positions
    tiger_reachable.erase(current.get_tiger().location);

    return tiger_reachable;
}


bool Smart_Mover::leads_to_cycle(Move_t mv) {
    bool cycling = false;
    // see if cycling
    Point_t net_mvmt = ZERO_VECT;
    Point_t mvmt_todo = mv.destination - mv.token.location;
    for(auto prev = prev_moves.rbegin(); prev != prev_moves.rend() &&
                                         !cycling; ++prev) {
        net_mvmt += prev->destination;
        net_mvmt -= prev->token.location;
        if(net_mvmt - mvmt_todo == ZERO_VECT) {
            cycling = true;
        }
    }
    return cycling;
}



Move_t Smart_Mover::finish_off_tiger() {
    Move_t to_do = NULL_MOVE;

    // if have cached moves, do it
    if(!q.empty()) {
        to_do = q.front();
        q.pop();

        // record move
        if(prev_moves.size() > MAX_CYCLING_CHECK) {
            prev_moves.pop_front();
        }
        prev_moves.push_back(to_do);

        return to_do;
    }

    set<Point_t> tiger_reachable = tiger_reachable_pos();
    unsigned long num_reachable = tiger_reachable.size();
    // look for moves into tiger reachable positions
    // and for moves that fill in behind those ones
    vector<Move_t> moves_into_t_reachable;
    vector<set<Move_t> >fill_in_behind_mvs; // map from a move into
    // a tiger reachable pos
    // to moves that fill in
    // behind it
    // for each man
    State cur = current;
    for(int i = 1; i < (int)cur.size(); ++i) {
        Token_t man = cur[i];
        // get man moves
        auto moves = game.validMoves(current, man);
        // see if moves into t_reachable
        for(int j = 0; j < moves.second.second; ++j) {
            Point_t dest = moves.first[j];
            if(tiger_reachable.find(dest) != tiger_reachable.end()) {
                Move_t mv = make_move(man, dest);
                moves_into_t_reachable.emplace_back(mv);
                fill_in_behind_mvs.emplace_back(set<Move_t>());
                // look for any moves that fill in behind it
                current.do_move(mv);
                for(int k = 1; k < (int)cur.size(); ++k) {
                    Token_t next_man = cur[k];
                    // don't look at man moving right now
                    if(next_man.location != mv.destination) {
                        // see if moves into man positions
                       Move_t fill_in = make_move(next_man, mv.token.location);
                        // if fills in behind the mv, add to set of Moves that
                        // fill in behind it
                        if(game.isValidMove(current, fill_in)) {
                            fill_in_behind_mvs.back().insert(fill_in);
                        }
                    }
                }
                current.do_move(-mv);
            }
        }

        // free memory
        if(moves.first) {
            delete[] moves.first;
            moves.first = nullptr;
        }
        if(moves.second.first) {
            delete [] moves.second.first;
            moves.second.first = nullptr;
        }

    }

    // try each pair of move / fill_in behind it to see if any reduces
    // tiger reachable positions
    bool move_found = false;
    for(int i = 0; i < (int)moves_into_t_reachable.size() && !move_found; ++i) {
        Move_t mv = moves_into_t_reachable[i];
        current.do_move(mv);
        unsigned long new_reachable = tiger_reachable_pos().size();
        if(new_reachable < num_reachable || (new_reachable == num_reachable &&
                                             mv.destination.row <
                                             mv.token.location.row)) {
            bool cycling = leads_to_cycle(mv);
            bool sec = secure(&current, &game);
            if(sec && !cycling) {
                move_found = true;
                q.push(mv);
            }
        }
        auto s = fill_in_behind_mvs[i].begin();
        while(s != fill_in_behind_mvs[i].end() && !move_found) {
            Move_t mv2 = *s;
            // if tiger can prevent this move, not a good move
            Move_t prevent_mv2 = make_move(current.get_tiger(), mv2.destination);
            bool preventable = game.isValidMove(current, prevent_mv2);
            if(!preventable && !tiger_can_jump(&current, &game)) {
                current.do_move(mv2);
                // if tiger cannot jump after and does not increase reachable positions,
                // do it
                new_reachable = tiger_reachable_pos().size();
                if (!tiger_can_jump(&current, &game) &&
                    (new_reachable < num_reachable || (new_reachable == num_reachable
                    && mv.destination.row < mv.token.location.row
                    && mv2.destination.row < mv2.token.location.row))) {
                    bool sec = secure(&current, &game);
                    bool cycling = leads_to_cycle(mv2);

                    if(sec && !cycling) {
                        move_found = true;
                        q.push(mv);
                        q.push(mv2);
                    }
                }
                current.do_move(-mv2);
            }
            ++s;
        }
        current.do_move(-mv);
    }

    // if no move found, then find the man furthest from tiger
    // in one norm with a move that decreases their distance and do it
    Move_t towards_tiger = NULL_MOVE;
    if(!move_found) {
        Token_t tiger = current.get_tiger();
        for(int i = 1; i < (int)cur.size() && !move_found; ++i) {
            Token_t man = cur[i];
            auto moves = game.validMoves(current, man);
            for (int j = 0; j < moves.second.second; ++j) {
                Point_t dest = moves.first[j];
                Move_t mv = make_move(man, dest);
                bool cycling = leads_to_cycle(mv);
                // if move moves the man closer to the tiger
                if(one_norm(tiger.location - dest) <
                   one_norm(tiger.location - man.location) && !cycling) {
                    towards_tiger = mv;
                    move_found = true;
                }
            }
            if(moves.first) {
                delete [] moves.first;
                moves.first = nullptr;
            }
            if(moves.second.first) {
                delete [] moves.second.first;
                moves.second.first = nullptr;
            }
        }
        if(move_found) {
            q.push(towards_tiger);
        }
    }

    if(!q.empty()) {
        to_do = q.front();
        q.pop();

        // record move
        if(prev_moves.size() > MAX_CYCLING_CHECK) {
            prev_moves.pop_front();
        }
        prev_moves.push_back(to_do);
    }


    return to_do;
}


Smart_Mover::Smart_Mover(const State& s):Men_Mover(s){
    back_row = NUM_ROW - 1;
    front_row = back_row - 2;
    special_moves = Specific_Move_Handler(&current, &game);

    // about next move
    move_ready = false;
    off_move_ready = false;
    path.clear();
    desired.clear();

    // about off move
    off_move_active = false;
    off_move = NULL_MOVE;

    // whether or not to tiger
    to_tiger_cage = false;
}

bool Specific_Move_Handler::one_col_two_back() {
    /// verify that in this position
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    bool in_pos = false;
    int back_col = -1;
    // in position if every column except for one is in back_row - 3 and
    // back_row - 2, and that other column is in back_row and back_row - 1
    // and the tiger is in (back_row - 3, back_col)
    if(row_to_col[back_row].size() == 1 &&
       row_to_col[back_row - 1].size() == 1 &&
       row_to_col[back_row - 2].size() == NUM_COL - 1 &&
       row_to_col[back_row - 3].size() == NUM_COL - 1 && front_row >= 4) {

        if(*row_to_col[back_row].begin() == *row_to_col[back_row - 1].begin()){
            in_pos = true;
            back_col = *row_to_col[back_row].begin();
        }
       if(current->get_tiger().location != make_point(back_row - 3, back_col)){
            in_pos = false;
        }

        for(int r = 0; r < back_row - 1 && in_pos; ++r) {
            if(row_to_col[r].find(back_col) != row_to_col[r].end()) {
                in_pos = false;
            }
        }
    }
    // if back col has an off move, not in position
    if(back_col == off_move.destination.col) {
        in_pos = false;
    }

    if(in_pos) {
        int help_col = back_col - 1;
        if(back_col < 4) {
            help_col = back_col + 1;
        }
        Point_t from, to;
        // move under tiger
        from = make_point(back_row - 2, help_col);
        to = make_point(back_row - 2, back_col);
        response.push(make_move(make_man(from),to));
        // move middle guy in column now holding three to col where higher guy
        // shifted from
        from = make_point(back_row - 1, back_col);
        to = make_point(back_row - 1, help_col);
        response.push(make_move(make_man(from),to));
        // move that guy up now
        /**from = make_point(back_row, help_col);
        to = make_point(back_row - 1, help_col);
        response.push(make_move(make_man(from),to));*/
    }

    return in_pos;
}


bool Specific_Move_Handler::three_by_diag() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = false;
    int left_col = -1;
    // if at tiger cage or not secure, return false
    if(front_row <= 4 || //(row_to_col[front_row].size() < 4 &&
       //front_row > 6) || (front_row <= 6 &&
       !secure(current, g)){//)) {
        return false;
    }

    int front_row_diag_cols[2];

    // compute diagonals
    if(front_row <= 8) {
        front_row_diag_cols[0] = front_row;
    }
    else {
        front_row_diag_cols[0] = (front_row + 1) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    Point_t tig_pos = current->get_tiger().location;
    for(int j = 0; j < 2 && left_col < 0; ++j) {
        in_pos = true;
        int c = front_row_diag_cols[j];
        // if tiger to left or right of diagonal, then not in position
        if(abs(tig_pos - make_point(front_row, c)) == make_point(0,1)) {
            in_pos = false;
        }
        // if tiger on the diagonal, not in position
        if(tig_pos == make_point(front_row, c)) {
            in_pos = false;
        }
        if((front_row < 8 && c < 4) || (front_row >= 8 && c >= 4)) {
            c -= 2;
        }
        // c, c + 1, c+ 2 should be three consecutive columns
        // with the diagonal intersecting the front_row at
        // either c or c + 1, and continues down into c
        // if off_col in [c, c+2] then not in pos
        if(c <= off_move.destination.col && c + 2 >= off_move.destination.col){
            in_pos = false;
        }
        for(int i = 0; i < 3 && in_pos; ++i) {
            if(c + i < 0 || c + i >= NUM_COL) {
                continue;
            }
            if(current->rows_in_col(c + i).size() != 2) {
                in_pos = false;
            }
            if(current->rows_in_col(c + i).find(back_row) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
            if(current->rows_in_col(c + i).find(back_row - 1) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
        }
        if(in_pos) {
            left_col = c;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int diag_col, lim_col, mid_col;
        if(left_col == front_row_diag_cols[0] ||
           left_col == front_row_diag_cols[1]) {
            diag_col = left_col;
            lim_col = left_col + 2;
        }
        else {
            lim_col = left_col;
            diag_col = left_col + 2;
        }
        mid_col = (lim_col + diag_col) / 2;
        Point_t from, to;
        if(tig_pos.col == diag_col) {
            from = make_point(back_row - 1, mid_col);
            to = make_point(back_row - 2, diag_col);
            this->response.push(make_move(make_man(from),to));
            from = make_point(back_row - 1, diag_col);
            to = make_point(back_row - 1, mid_col);
        }
        else {
            from = make_point(back_row - 1, diag_col);
            to = make_point(back_row - 2, diag_col);
            if(abs(tig_pos.col - diag_col) > 1) {
                this->response.push(make_move(make_man(from),to));
                from = make_point(back_row, diag_col);
                to = make_point(back_row - 1, diag_col);
            }
        }
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::near_top_finish_three_by_diag() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = false;
    int left_col = -1;
    // if at tiger cage or not near front return false
    if(front_row <= 4){// || (front_row > 6)) {
        return false;
    }

    int front_row_diag_cols[2];
    // compute diagonals
    if(front_row <= 8) {
        front_row_diag_cols[0] = front_row;
    }
    else {
        front_row_diag_cols[0] = (front_row + 1) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    Point_t tig_pos = current->get_tiger().location;
    for(int j = 0; j < 2 && left_col < 0; ++j) {
        in_pos = true;
        int c = front_row_diag_cols[j];
        // if haven't started a three by diag special move, not in position
        if(!current->is_occupied(make_point(front_row, c)) ||
           tig_pos == make_point(front_row,c)){
            in_pos = false;
        }
        // c, c + 1, c+ 2 should be three consecutive columns
        // with the diagonal intersecting the front_row at
        // either c or c + 1, and continues down into c
        // if any of those has an off move, then not in position
        if(c <= off_move.destination.col && c + 2 >= off_move.destination.col) {
            in_pos = false;
        }
        for(int i = 0; i < 3 && in_pos; ++i) {
            if(c + i < 0 || c + i >= NUM_COL) {
                continue;
            }
            if(current->rows_in_col(c + i).size() != 2) {
                in_pos = false;
            }
            // if don't have back row filled in each column, not in pos
            if(current->rows_in_col(c + i).find(back_row) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
            // if dont have row above back row filled in each column except
            // that with the diagonal, not in pos
            if(c + i != front_row_diag_cols[j]) {
                if (current->rows_in_col(c + i).find(back_row - 1) ==
                    current->rows_in_col(c + i).end()) {
                    in_pos = false;
                }
            }
        }
        if(in_pos) {
            left_col = c;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int diag_col;// lim_col;// mid_col;
        if(left_col == front_row_diag_cols[0] ||
           left_col == front_row_diag_cols[1]) {
            diag_col = left_col;
           // lim_col = left_col + 2;
        }
        else {
         //   lim_col = left_col;
            diag_col = left_col + 2;
        }
        //mid_col = (lim_col + diag_col) / 2;
        Point_t from, to;
        from = make_point(back_row, diag_col);
        to = make_point(back_row - 1, diag_col);
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::lagging_col() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = true;
    int lag_col = -1;
    // if at tiger cage return false
    if(front_row <= 4) {
        return false;
    }
    int front_row_diag_cols[2];

    // compute diagonals of row above front row
    if(front_row - 1 <= 8) {
        front_row_diag_cols[0] = front_row - 1;
    }
    else {
        front_row_diag_cols[0] = (front_row) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    // if can't have a lagging column by the number of men in each column,
    // not in position
    if(row_to_col[front_row].size() != NUM_COL - 1 ||
       row_to_col[front_row+1].size() != NUM_COL ||
       row_to_col[front_row + 2].size() != 1) {
        in_pos = false;
    }
    else {
        lag_col = *row_to_col[front_row + 2].begin();
        if(row_to_col[front_row].find(lag_col) != row_to_col[front_row].end()) {
            in_pos = false;
        }
        // if lagging col has an off move, not in position
        if(lag_col == off_move.destination.col) {
            in_pos = false;
        }
    }


    // if the lagging column is the middle on the lower half of the board
    // or one of the edges on the upper half, then it is fine to lag
    // and we do not need to do any special moves
    if(front_row >= 8 && lag_col == (NUM_COL - 1) / 2 ) {
        in_pos = false;
    }
    else if(front_row < 8 && (lag_col == 0 || lag_col == NUM_COL - 1)) {
        in_pos = false;
    }



    // if tiger not suppressing lagging column, i.e. not
    // right above it/able to get righ above it, then not lagging
    Token_t tiger = current->get_tiger();
    Point_t tig_pos = tiger.location;
    if(tig_pos != make_point(front_row, lag_col) &&
       !g->isValidMove(*current,
                       make_move(tiger, make_point(front_row, lag_col)))){
        in_pos = false;
    }

    bool protect_diag_from_back = false; // true if it will be necessary
    // to protect from a diagonal attack when switching the lagging col
    // to an appropriate position
    if(lag_col == front_row_diag_cols[0] || lag_col == front_row_diag_cols[1]) {
        // on row 7 don't need to worry about the diagonals for this move
        if(front_row != 8 && front_row != 7) {
            protect_diag_from_back = true;
        }
    }

    // if protecting a diagonal, need the tiger in line with the men
    // else, need room below him to switch into
    if(protect_diag_from_back) {
        if(tig_pos.row != front_row) {
            in_pos = false;
        }
    }
    else {
        if(tig_pos.row != front_row - 1) {
            in_pos = false;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int col_in, /*col_2_in,*/ col_3_in;
        if((lag_col < 4 && front_row >= 8) || (lag_col >= 4 && front_row < 8)) {
            col_in = min(tig_pos.col + 1, NUM_COL - 1);
           // col_2_in = min(tig_pos.col + 2, NUM_COL - 1);
            col_3_in = min(tig_pos.col + 3, NUM_COL - 1);
        }
        else {
            col_in = max(0, tig_pos.col - 1);
           // col_2_in = max(0, tig_pos.col - 2);
            col_3_in = max(0, tig_pos.col - 3);
        }

        /*
        if(col_3_in == col_2_in && protect_diag_from_back) {
            return false;
        }
        */

        int tiger_col = tig_pos.col;
        Point_t from, to;
        if(protect_diag_from_back) {
            from = make_point(front_row + 1, col_3_in);
            to = make_point(front_row + 2, col_3_in);
            this->response.push(make_move(make_man(from),to));
        }
        // switch with below tiger
        from = make_point(front_row, col_in);
        to = make_point(front_row, tiger_col);
        this->response.push(make_move(make_man(from),to));
        from = make_point(front_row + 2, tiger_col);
        to = make_point(front_row + 2, col_in);
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::try_switch() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    Token_t tiger = current->get_tiger();
    bool in_pos = false;

    // if at tiger cage or not enough moved up or,
    // or if tiger not above front row or if front row and back row far apart,
    // not in this position
    if(front_row <= 4 || row_to_col[front_row].size() < NUM_COL - 1 ||
       tiger.location.row != front_row - 1 || (back_row - front_row) > 3) {
        return false;
    }

    int tiger_col = tiger.location.col;

    // if tiger not adjacent to diagonal on front row, then return false
    int diag_threatening = -1;
    Point_t tig_pos = tiger.location;
    int diag;
    if(tig_pos.row + 1 <= 8) {
        diag = tig_pos.row + 1;
        Point_t to = make_point(tig_pos.row + 1, diag);
        if(g->isValidMove(*current, make_move(tiger, to))){
            diag_threatening = diag + 1;
        }
        else {
            diag = NUM_COL - 1 - (tig_pos.row + 1);
            Point_t to = make_point(tig_pos.row + 1, diag);
            if(g->isValidMove(*current, make_move(tiger, to))){
                diag_threatening = diag - 1;
            }
        }
    }
    else {
        diag = (tig_pos.row + 2) % NUM_COL;
        Point_t to = make_point(tig_pos.row + 1, diag);
        if(g->isValidMove(*current, make_move(tiger, to))){
            diag_threatening = diag + 1;
        }
        else {
            diag = NUM_COL - 1 - (tig_pos.row + 2) % NUM_COL;
            Point_t to = make_point(tig_pos.row + 1, diag);
            if(g->isValidMove(*current, make_move(tiger, to))){
                diag_threatening = diag - 1;
            }
        }
    }

    // if no help col, return false
    if(diag_threatening < 0 || diag_threatening >= NUM_COL) {
        return false;
    }
    // if either column has an off move, not in a special move
    if(diag_threatening == off_move.destination.col ||
       tiger_col == off_move.destination.col) {
        return false;
    }

    // else, perform the switch on the help_col
    in_pos = true;
    Point_t from, to;
    // move under tiger
    from = make_point(front_row, diag_threatening);
    to = make_point(front_row, tiger_col);
    response.push(make_move(make_man(from),to));
    // move guy protecting diag up
    from = make_point(front_row + 2, diag_threatening +
                                     (diag_threatening - tiger_col));
    to = make_point(front_row + 1, diag_threatening +
                                   (diag_threatening - tiger_col));
    response.push(make_move(make_man(from),to));
    // protect him
    from = make_point(front_row + 2, tiger_col);
    to = make_point(front_row + 2, diag_threatening);
    response.push(make_move(make_man(from),to));

    return in_pos;
}


Specific_Move_Handler::Specific_Move_Handler(Unordered_State* st,
                                             GameRunner* g) {
    this->current = st;
    this->g = g;
    this->front_row = front_row;
    this->back_row = back_row;
}


bool Specific_Move_Handler::handle_special_case(Move_t off_move){
    if(front_row <= 4) {
        return false;
    }
    this->off_move = off_move;
    // pretend no off move
    bool undid_off = false;
    if(g->isValidMove(*current, -off_move)) {
        undid_off = true;
        current->do_move(-off_move);
    }

    bool in_specific_case = false;
    in_specific_case = one_col_two_back();
    if(!in_specific_case) {
        in_specific_case = three_by_diag();
    }
    if(!in_specific_case) {
        in_specific_case = this->near_top_finish_three_by_diag();
    }
    if(!in_specific_case) {
        in_specific_case = lagging_col();
    }
    if(!in_specific_case) {
        in_specific_case = try_switch();
    }

    if(undid_off) {
        current->do_move(off_move);
    }

    return in_specific_case;
}


void Specific_Move_Handler::set_front_back_row(int front_row, int back_row){
    this->front_row = front_row;
    this->back_row = back_row;
}


Move_t Specific_Move_Handler::get_move() {
    Move_t to_do = NULL_MOVE;
    if(response.size() > 0) {
        to_do = response.front();
        response.pop();
    }
    return to_do;
}


void Specific_Move_Handler::clear_moves() {
    this->response = queue<Move_t>();
}


bool Transition::search_for_state(int max_moves, int num_moves) {
    Token_t tiger = current->get_tiger();
    // if already know result of this state return it (i.e. have
    // been to this state with this many or more moves remaining)
    pair<Hash_val, Token_t> ndx = make_pair(cur_hash_val, tiger);
    auto visited_ptr = visited.find(ndx);
    if(visited_ptr != visited.end()) {
        // if been here before with at least as many remaining moves and could
        // not get there, return false
        if(num_moves >= visited_ptr->second.first &&
           visited_ptr->second.second == false) {
            return visited_ptr->second.second;
        } // else if been here before with fewer remaining moves and could
            // get there, return true
        else if(num_moves <= visited_ptr->second.first &&
                visited_ptr->second.second == true){
            return visited_ptr->second.second;
        }
    }

    // see if at desired state
    bool desired_state_found = false;
    if(hash_vals->find(cur_hash_val) != hash_vals->end()) {
        desired_state_found = true;
    }


    // if at max moves or found a desired state, return
    if(desired_state_found || num_moves >= max_moves){
        visited[ndx] = make_pair(num_moves, desired_state_found);
        return desired_state_found;
    }

    // determine whether this state is any good, i.e. if there exists a
    // move which, no matter the tiger response, allows one to reach the
    // desired state
    bool any_good = false; // true iff path to desired state following above
    // guidelines

    // determine men to move
    vector<Token_t> to_move;
    for(int c = left_r_bds.first; c < left_r_bds.second; ++c) {
        for(auto r = current->rows_in_col(c).begin();
            r != current->rows_in_col(c).end(); ++r) {
            to_move.push_back(make_man(make_point(*r,c)));
        }
    }

    // for each man
    for(unsigned int i = 0; i < to_move.size() && !any_good; ++i) {
        Token_t man = to_move[i];
        auto man_moves = game->validMoves(*current, man);
        for(int m = 0; m <  man_moves.second.second && !any_good; ++m){
            bool m_okay = true; // true iff this move is a good response to
            // the current state
            Move_t mv = make_move(man, man_moves.first[m]);
            // don't move out of bounds
            if(mv.destination.row < low_high_bds.first ||
               mv.destination.row >= low_high_bds.second ||
               mv.destination.col < left_r_bds.first ||
               mv.destination.col >= left_r_bds.second) {
                m_okay = false;
                /// DO THIS BETER ????
                continue;
            }
            // perform move
            cur_hash_val = next_hash(mv, cur_hash_val, back_row);

            current->do_move(mv);
            // if tiger can capture after move, this is a bad move
            if(tiger_can_jump(current, game)) {
                visited.insert(make_pair(make_pair(cur_hash_val, tiger),
                                         make_pair(0, false)));
                m_okay = false;
            }
            // check each tiger response
            auto tiger_moves = game->validMoves(*current, tiger);
            for(auto t = 0; t < tiger_moves.second.second && m_okay; ++t) {
                // move tiger
                Move_t t_mv = make_move(tiger, tiger_moves.first[t]);
                current->do_move(t_mv);
                if(!search_for_state(max_moves, num_moves + 1)) {
                    m_okay = false;
                }
                // undo tiger move
                current->do_move(-t_mv);
            }
            // free memory
            if(tiger_moves.second.first) {
                delete[] tiger_moves.second.first;
                tiger_moves.second.first = nullptr;
            }
            if(tiger_moves.first) {
                delete[] tiger_moves.first;
                tiger_moves.first = nullptr;
            }
            // undo move
            current->do_move(-mv);
            cur_hash_val = next_hash(-mv, cur_hash_val, back_row);
            // if move is okay, record it
            if(m_okay){
                any_good = true;
                path[ndx] = mv;
            }
        }
        // free memory
        if(man_moves.second.first) {
            delete[] man_moves.second.first;
            man_moves.second.first = nullptr;
        }
        if(man_moves.first) {
            delete[] man_moves.first;
            man_moves.first = nullptr;
        }
    }

    visited[ndx] = make_pair(num_moves, any_good); // record answer
    return any_good;
}


Transition::Transition(Unordered_State* c, set<Hash_val>*hash_vals,
                       GameRunner*game,
                       int back_row, pair<int,int> left_right_bds,
                       pair<int,int> low_high_bds) {

    this->current = c;
    this->hash_vals = hash_vals;
    this->game = game;
    this->back_row = back_row;
    this->cur_hash_val = hash_locs(*c, back_row);
    this->left_r_bds = left_right_bds;
    this->low_high_bds = low_high_bds;
    this->path.clear();
    this->visited.clear();
}


bool Transition::find_path_to_state(int max_moves, int minimum_max_moves){
    // see if can reach a desired state "without moving" this time
    // this prevents degenerating into cycles
    bool found = false;
    for(int i = max_moves - max(1, minimum_max_moves); i >= 0 && !found; --i) {
        this->visited.clear();
        this->path.clear();
        if(search_for_state(max_moves, i)) {
            found = true;
        }
    }
    return found;
}


map<pair<Hash_val,Token_t>, Move_t> Transition::get_path() {
    return this->path;
}
#endif //DEEP_BLUE_GROUP_PROJECT_H
