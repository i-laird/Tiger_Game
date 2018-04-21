#include "general_util.h"

#ifndef UNORDERDED_STATE_H_INCLUDED
#define UNORDERDED_STATE_H_INCLUDED

/// State that is not affected by order of men tokens
class Unordered_State{
    private:
        Token_t tiger;
        set<int> col_to_rows[NUM_COL];
    public:
        /// constructors
        Unordered_State();
        Unordered_State(const State& st);
        /// comparators
        bool operator<(const Unordered_State& that) const;
        bool operator>(const Unordered_State& that) const;
        bool operator<=(const Unordered_State& that) const;
        bool operator>=(const Unordered_State& that) const;
        bool operator==(const Unordered_State& that) const;
        bool operator!=(const Unordered_State& that) const;
        /// accessors
        const Token_t& get_tiger() const;
        const set<int>& rows_in_col(int col) const;
        /// mutators
        void set_tiger(const Token_t& t);
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

        /*                   men_in_grid
        *
        * description: Returns a set of the locations of all men in a given
        *              subgrid of the board
        * return: set<Point_t>&&
        * precondition: This is a valid Unordered state object
        * postcondition: Returns a set of men locations which have the following
        *                properties:
        *                - low <= row <= high
        *                - left <= col <= right
        */
        set<Point_t> men_in_grid(int low, int high, int left, int right) const;
        operator State() const;

};



#endif // UNORDERDED_STATE_H_INCLUDED
