#include "general_util.h"
#include <algorithm>

#ifndef UNORDERDED_STATE_H_INCLUDED
#define UNORDERDED_STATE_H_INCLUDED

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



#endif // UNORDERDED_STATE_H_INCLUDED
