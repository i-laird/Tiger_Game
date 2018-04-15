#include "testing_util.h"

#include "general_util.h"
#include "Unorderded_State.h"
#include "game_runner.h"
#include <queue>
#include <cassert>
using namespace std;

#ifndef MEN_UTIL_H_INCLUDED
#define MEN_UTIL_H_INCLUDED

typedef pair<int,int> Hash_val;

Hash_val hash_locs(const Unordered_State& st, int back_row);

Hash_val next_hash(const Move_t& m, Hash_val old_hash, int back_row);

bool tiger_can_jump(const Unordered_State * st, GameRunner *g);

bool secure(Unordered_State* st, GameRunner* g, Move_t off_move = NULL_MOVE);

#endif // MEN_UTIL_H_INCLUDED
