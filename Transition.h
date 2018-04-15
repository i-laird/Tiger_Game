#include "testing_util.h"
#include "men_util.h"

#ifndef TRANSITION_H_INCLUDED
#define TRANSITION_H_INCLUDED


class Transition {
    private:
        Unordered_State* current;
        set<Hash_val>* hash_vals;
        GameRunner * game;
        int back_row;
        pair<int,int> left_r_bds;
        pair<int,int> low_high_bds;
        map<pair<Hash_val,Token_t>, pair<int,bool>> visited;// map to num moves
        // this state was last seen at and whether or not was true
        map<pair<Hash_val,Token_t>, Move_t> path;
        Hash_val cur_hash_val;

        bool search_for_state(int max_moves, int num_moves = 0);
    public:
        Transition(Unordered_State* c, set<Hash_val>*hash_vals,GameRunner*game,
                   int back_row, pair<int,int> left_right_bds,
                   pair<int,int> low_high_bds);
        bool find_path_to_state(int max_moves);
        map<pair<Hash_val,Token_t>, Move_t> get_path();
};


#endif // TRANSITION_H_INCLUDED
