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
    pair<Point_t *, pair<bool*,int>> t_moves;
    t_moves = g->validMoves(*st, st->get_tiger());

    bool jump = false;
    for(int t = 0; t < t_moves.second.second && !jump; ++t) {
        if(t_moves.second.first[t]){
            jump = true;
        }
    }
    delete [] t_moves.first;
    delete [] t_moves.second.first;
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
    pair<Point_t*, pair<bool*, int> > t_moves;
    while(!frontier.empty() && !jump) {
        current = frontier.front();
        // mark visited
        st->set_tiger(make_tiger(current));
        frontier.pop();

        // if tiger can jump, then this is not secure
        jump = tiger_can_jump(st, g);

        // else, add each reachable new position to the frontier
        if(!jump) {
            t_moves = g->validMoves(*st, make_tiger(current));
            Point_t to;
            for(int t = 0; t < t_moves.second.second; ++t) {
                to = t_moves.first[t];
                if(!visited[to.row][to.col]) {
                    visited[to.row][to.col] = true;
                    frontier.push(to);
                }
            }
            // free memory
            delete [] t_moves.first;
            delete [] t_moves.second.first;
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


Move_t bfs_move_getter(Unordered_State* st, GameRunner* g, Point_t from) {

	queue<Point_t> frontier;

	bool visited[NUM_ROW][NUM_COL];
	int dist[NUM_ROW][NUM_COL];
	Point_t pred[NUM_ROW][NUM_COL];
	for (int r = 0; r < NUM_ROW; ++r) {
		for (int c = 0; c < NUM_COL; ++c) {
			visited[r][c] = false;
			dist[r][c] = INFTY;
			pred[r][c] = NULL_POINT;
		}
	}

	Point_t current = from;
	visited[current.row][current.col] = true;
	dist[current.row][current.col] = 0;
	frontier.push(current);
	pair<Point_t*, pair<bool*, int> > moves;
	while (!frontier.empty()) {
		current = frontier.front();
		// mark visited
		frontier.pop();

		// add each reachable new position to the frontier
		moves = g->validMoves(*st, make_man(current));
		Point_t to;
		for (int t = 0; t < moves.second.second; ++t) {
			to = moves.first[t];
			if (!visited[to.row][to.col]) {
				visited[to.row][to.col] = true;
				dist[to.row][to.col] = dist[current.row][current.col] + 1;
				pred[to.row][to.col] = current;
				frontier.push(to);
			}
		}
		// free memory
		if (moves.first)
			delete[] moves.first;
		if (moves.second.first)
			delete[] moves.second.first;
	}

	set<pair<int, int>> bad_locs;
	for (int i = 0; i < 8; i++) {
		bad_locs.insert(KILL[i].row, KILL[i].col);
	}

	Point_t to = NULL_POINT;
	int min_len = INFTY;
	for (int i = 0; i < NUM_ROW; i++) {
		for (int j = 0; j < NUM_COL; j++) {
			if (dist[i][j] < min_len && 
				bad_locs.find(make_pair(i, j)) == bad_locs.end()) {
				min_len = dist[i][j];
				to = make_point(i, j);
			}
		}
	}

	if (to == NULL_POINT) {
		return NULL_MOVE;
	}

	while (pred[to.row][to.col] != NULL_POINT) {
		to = pred[to.row][to.col];
	}

	return make_move(make_man(from), to);
}


