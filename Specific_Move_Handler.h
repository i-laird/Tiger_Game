#include <iostream>

#include "Unordered_State.h"
#include "game_runner.h"
#include "men_util.h"
#include "constants.h"
#include <queue>

using namespace std;

#ifndef SPECIFIC_MOVE_HANDLER_H_INCLUDED
#define SPECIFIC_MOVE_HANDLER_H_INCLUDED

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


#endif // SPECIFIC_CASE_HANDLER_H_INCLUDED
