#include "general_util.h"
#include "Unordered_State.h"
#include "game_runner.h"

#include <queue>
#include <cassert>
using namespace std;

#ifndef MEN_UTIL_H_INCLUDED
#define MEN_UTIL_H_INCLUDED

// these points are those which should be occupied in order to
// keep the tiger in the cage
const Point_t STAGE_POSITIONS[8] = { make_point(4, 3),
						make_point(4, 4), 
						make_point(4, 5), 
						make_point(5, 3), 
						make_point(5, 4), 
						make_point(5, 5), 
						make_point(6, 2), 
						make_point(6, 6) };

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

Move_t bfs_move_getter(Unordered_State* st, GameRunner* g, Point_t from);

#endif // MEN_UTIL_H_INCLUDED
