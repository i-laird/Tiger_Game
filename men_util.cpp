#include "men_util.h"

Hash_val hash_locs(const Unordered_State& st, int back_row) {
    Hash_val h = make_pair(0,0);
    // for each man
    for(int c = 0; c < NUM_COL; ++c) {
        for(auto r = st.rows_in_col(c).begin(); r != st.rows_in_col(c).end(); ++r) {
            Point_t loc = make_point(*r, c);
            int val = pow(2, loc.col + + ((back_row - loc.row) % 2) * NUM_COL);

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

// returns the move from the man closest to dest which moves him
// towards dest as quickly as possible
//
// returns  NULL_MOVE if no such move exists or if from is occupied
Move_t bfs_move_getter(Unordered_State* st, Point_t dest) {
    // if from is occupied, don't try to move towards it
    if(st->is_occupied(dest)) {
        return NULL_MOVE;
    }

	queue<Point_t> frontier;

	bool visited[NUM_ROW][NUM_COL];
	int dist[NUM_ROW][NUM_COL];
	// initialize visited to false, dist to infinity, and predecessor to null
	for (int r = 0; r < NUM_ROW; ++r) {
		for (int c = 0; c < NUM_COL; ++c) {
			visited[r][c] = false;
			dist[r][c] = INFTY;
		}
	}

    set<Point_t> bad_locs;
    // we don't want to move any men that are needed to stage the tiger cage
    for (int i = 0; i < STAGE_POS_SIZE; i++) {
        bad_locs.insert(STAGE_POSITIONS[i]);
    }

	// initialize current
	Point_t current = dest;
	visited[current.row][current.col] = true;
	dist[current.row][current.col] = 0;
	frontier.push(current);
	pair<Point_t*, pair<bool*, int> > moves;
	// perform a bfs from the dest without using diagonals
    Point_t dirs[4] = {UP, DOWN, LEFT, RIGHT};
    Move_t mv_todo = NULL_MOVE;
	while (!frontier.empty() && mv_todo == NULL_MOVE) {
		current = frontier.front();
		// mark visited
		frontier.pop();

		// add each reachable new position to the frontier
		for(int i = 0; i < 4 && mv_todo == NULL_MOVE; ++i) {
		    // if not a populated point and in bounds and unvisited,
            Point_t to = current + dirs[i];
            // if to is in tiger cage, don't record it
            if(to.row < CAGE_ENTRANCE.row) {
                continue;
            }
		    if(!visited[to.row][to.col] && !st->is_occupied(to)) {
		        // don't need to check tiger cage because not using diagonals
		        if((to.row >= 4 && to.col >= 0 && to.col < NUM_COL)) {
		            visited[to.row][to.col] = true;
		            dist[to.row][to.col] = dist[current.row][current.col] + 1;
		            frontier.push(to);
		        }
		    }
		    // if found a man not in a bad loc, move him to the predecessor
		    else if(st->is_occupied(to) && st->get_tiger() != make_tiger(to) &&
                    bad_locs.find(to) == bad_locs.end()) {
		        mv_todo = make_move(make_man(to), current);
		    }
		}
	}

	return mv_todo;
}


