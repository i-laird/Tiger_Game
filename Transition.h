#include "testing_util.h"
#include "men_util.h"

#include <utility>
using namespace std;

#ifndef TRANSITION_H_INCLUDED
#define TRANSITION_H_INCLUDED


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


#endif // TRANSITION_H_INCLUDED
