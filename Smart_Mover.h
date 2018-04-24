#ifndef TIGER_GAME_SMART_MOVER_H
#define TIGER_GAME_SMART_MOVER_H

#include <iostream>
#include "Transition.h"
#include "Specific_Move_Handler.h"
#include "testing_util.h"
#include "Men_Mover.h"
#include <queue>

class Smart_Mover : public Men_Mover {
private:
	// fun times with queues
	queue<Move_t> q;

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

		Move_t kill_tiger_handling();

		bool cage_staged();

		Move_t stage_men();

		Move_t get_move_in_cage();

		Move_t get_move_into_cage();

		set<Point_t> tiger_reachable_pos();
		Move_t finish_off_tiger();

    public:
    /*       Men_Mover(const State& s)
     *
     * description: creates a Men_Mover object with the given State
     *              used to create current
     * return: none
     * precondition: s is a valid State
     * postcondition: a Men_Mover object is created
     */
    Smart_Mover(const State& s);

protected:
    /*       next_move(Move_t tiger_move)
     *
     * description: performs the tiger_move on current, then
     *              returns the best move for the men to perform
     *              in order to prevent capture of men and
     *              move men forward
     *                performs that move on current and returns it
     * return: Move_t
     * precondition: none
     * postcondition: the returned move is performed on current, any
     *                moves calculated turns in advanced are stored
     */
    Move_t execute_move() override;
};


#endif //TIGER_GAME_SMART_MOVER_H
