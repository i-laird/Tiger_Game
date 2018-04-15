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

