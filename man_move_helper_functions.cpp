#include "man_move_helper_functions.h"

bool operator<(const Token_t & a, const Token_t & b) {
    return a.location < b.location;
}


Point_t make_point(int r, int c) {
    Point_t p;
    p.row = r;
    p.col = c;
    return p;
}

bool man_pos_in(const State& s, const set<State>& t) {
    bool in = false;
    for(auto j = t.begin(); j != t.end() && !in; ++j) {
        if(s.size() == j->size()) {
            set<Point_t> s_positions, t_positions;
            for(unsigned int i = 0; i < s.size(); ++i) {
                if(s[i].color != RED) {
                    s_positions.insert(s[i].location);
                }
                if((*j)[i].color != RED) {
                    t_positions.insert((*j)[i].location);
                }
            }
            in = (s_positions == t_positions);
        }
    }
    return in;
}


bool secure_from_above(const State& s, int back_row, const Board& brd) {
    bool secure = true;
    int highest_row = 12;
    set<Point_t> men_positions;
    for(auto i = s.begin(); i != s.end(); ++i) {
        if(i->color != RED) {
            men_positions.insert(i->location);
            highest_row = min(highest_row, i->location.row);
        }
    }
    Token_t tiger;
    tiger.color = RED;

    State temp = s;
    // try in further and further front of each man until
    // row below tiger is empty
    bool done = false;
    for(int i = 1; i < 13 && !done && secure; ++i) {
        done = true;
        for(int j = 1; j < temp.size() && secure; ++j) {
            tiger.location = temp[j].location;
            tiger.location.row -= i;
            if(men_positions.find(tiger.location) == men_positions.end()) {
                temp[0] = tiger;
                secure = !tiger_can_jump(temp, brd);
            }
            // if row below tiger might have men, not done
            if(tiger.location.row > highest_row + 2) {
                done = false;
            }
        }
    }

    return secure;
}


Men_Move_F::Men_Move_F(int br){
    this->back_row = br;
}


bool tiger_can_jump(const State& s, const Board& brd){
    assert(s[0].color == RED);
    bool jump = false;
    // see if any potential jump is a valid move
    Move_t tiger_jump;
    tiger_jump.token = s[0];
    int sign[3] = {-1,0,1};
    for(int i = 0; i < 9 && !jump; ++i) {
        tiger_jump.destination = s[0].location;
        tiger_jump.destination.row += 2 * sign[i / 3];
        tiger_jump.destination.col += 2 * sign[i % 3];
        jump = brd.isValid(s, tiger_jump);
    }

    // return whether or not can jump
    return jump;
}


vector<pair<int, Move_t> >* Men_Move_F::operator()(State& s, const list<int>& to_move){
    // find back_row if not already found
    if(back_row == -1) {
        assert(to_move.size() >= 1);
        back_row = s[to_move.front()].location.row;
        for(auto i = to_move.begin(); i != to_move.end(); ++i) {
            back_row = min(back_row, s[*i].location.row);
        }
    }

    vector<pair<int, Move_t> >* suggested_moves = new vector<pair<int, Move_t> >;
    // for each man that might be moved
    Move_t new_move;
    for(unsigned int i = 0; i < to_move.size(); ++i) {
        // record previous location
        Point_t prev_loc = s[i].location;
        // create forward move
        new_move.destination = s[i].location;
        new_move.destination.row -= 1;
        new_move.token = s[i];
        s[i].location = new_move.destination;
        // if new space is invalid or too far forward, don't add to suggested moves
        if(new_move.destination.row >= back_row - 2 && brd.isValid(s, new_move)){
            // if tiger cannot jump after move, add to suggested moves
            if(!tiger_can_jump(s, this->brd)){
                suggested_moves->push_back(make_pair(i, new_move));
            }
        }
        // undo move
        s[i].location = prev_loc;
    }
    return suggested_moves;
}


Men_Move_FLR::Men_Move_FLR(int br){
    this->back_row = br;
}


vector<pair<int, Move_t> >* Men_Move_FLR::operator()(State& s, const list<int>& to_move){
    assert(to_move.size() >= 1);
    // find back_row if not already found
    if(back_row == -1) {
        back_row = s[to_move.front()].location.row;
        for(auto i = to_move.begin(); i != to_move.end(); ++i) {
            back_row = min(back_row, s[*i].location.row);
        }
    }
    // find left and right columns
    left_col = s[to_move.front()].location.col;
    for(auto i = to_move.begin(); i != to_move.end(); ++i) {
        left_col = min(left_col, s[*i].location.col);
    }
    right_col = s[to_move.front()].location.col;
    for(auto i = to_move.begin(); i != to_move.end(); ++i) {
        right_col = max(right_col, s[*i].location.col);
    }

    vector<pair<int, Move_t> >* suggested_moves = new vector<pair<int, Move_t> >;
    // for each man that might be moved
    Move_t f_move, left_move, r_move;
    Point_t prev_loc;
    for(unsigned int i = 0; i < to_move.size(); ++i) {
        // store previous location
        prev_loc = s[i].location;
        /// create forward move
        f_move.destination = s[i].location;
        f_move.destination.row -= 1;
        f_move.token = s[i];
        s[i].location = f_move.destination;
        // if move is valid and not too far forward
        if(f_move.destination.row >= back_row - 2 && brd.isValid(s, f_move)){
            // if tiger cannot jump after move
            if(!tiger_can_jump(s, this->brd)){
                // add move
                suggested_moves->push_back(make_pair(i, f_move));
            }
        }
        // undo move
        s[i].location = prev_loc;

        /// create left move
        left_move.destination = s[i].location;
        left_move.destination.col -= 1;
        left_move.token = s[i];
        s[i].location = left_move.destination;
        // if move is valid and not too far left
        if(left_move.destination.col >= left_col && brd.isValid(s, left_move)){
            // if tiger cannot jump after move
            if(!tiger_can_jump(s, this->brd)){
                // add move
                suggested_moves->push_back(make_pair(i, left_move));
            }
        }
        // undo move
        s[i].location = prev_loc;

        /// create right move
        r_move.destination = s[i].location;
        r_move.destination.col += 1;
        r_move.token = s[i];
        s[i].location = r_move.destination;
        // if move is valid and not too far left
        if(r_move.destination.col >= left_col && brd.isValid(s, r_move)){
            // if tiger cannot jump after move
            if(!tiger_can_jump(s, this->brd)){
                // add move
                suggested_moves->push_back(make_pair(i, r_move));
            }
        }
        // undo move
        s[i].location = prev_loc;
    }
    return suggested_moves;
}


vector<pair<int, Move_t> >* Tiger_Move::operator()(State& s, const list<int>&) {
    assert(s[0].color == RED);
    vector<pair<int, Move_t> >* tiger_moves = new vector<pair<int, Move_t> >;
    // try to move in each direction
    Move_t t_move;
    t_move.token = s[0];
    int sign[3] = {-1,0,1};
    for(int i = 0; i < 9; ++i) {
        // try moving and try jumping
        for(int j = 0; j < 2; ++j){
            t_move.destination = s[0].location;
            t_move.destination.row += sign[i / 3];
            t_move.destination.col += sign[i % 3];
            if(brd.isValid(s, t_move)){
                tiger_moves->push_back(make_pair(0,t_move));
            }
        }
    }

    return tiger_moves;
}

State_Transition empty_transition() {
    State_Transition t;
    t.current.clear();
    t.desired.clear();
    t.found_states.clear();
    t.to_move.clear();
    t.path.clear();
    t.max_moves = 0;
    t.num_moves = 0;
    t.back_row = 0;

    return t;
}



void p_board(ostream& out, const State& s) {
    map<Point_t, string> pts;
    // tiger cage
    pts[make_point(0,4)] = ".";
    pts[make_point(1,3)] = ".";
    pts[make_point(1,5)] = ".";
    pts[make_point(2,2)] = ".";
    pts[make_point(2,4)] = ".";
    pts[make_point(2,6)] = ".";
    pts[make_point(3,3)] = ".";
    pts[make_point(3,5)] = ".";
    for(int r = 4; r < 13; ++r) {
        for(int c = 0; c < 9; ++c) {
            pts[make_point(r,c)] = ".";
        }
    }

    for(auto i = s.begin(); i != s.end(); ++i) {
        if(i->color == RED) {
            pts[i->location] = "X";
        }
        else {
            pts[i->location] = "0";
        }
    }

    out << "        " << pts[make_point(0,4)] << "         \n";
    out << "       / \\        \n";
    out << "      " << pts[make_point(1,3)] << "   " << pts[make_point(1,5)] << "      \n";
    out << "     / \\ / \\      \n";
    out << "    " << pts[make_point(2,2)] << "   " << pts[make_point(2,4)];
    out << "   " << pts[make_point(2,6)] << "    \n";
    out << "     \\ / \\ /      \n";
    out << "      " << pts[make_point(3,3)] << "   " << pts[make_point(3,5)] << "      \n";
    out << "       \\ /        \n";
    int left_diag = 3;
    int right_diag = 4;
    for(int r = 4; r < 13; ++r) {
        for(int c = 0; c < 9; ++ c) {
            if(c < 8){
                out << pts[make_point(r,c)] << "-";
            }
            else {
                out << pts[make_point(r,c)];
            }
        }
        out << "\n";
        if(r < 12) {
            for(int c = 0; c < 9; ++ c) {
                if(c == left_diag){
                   out << "|/";
                }
                else if(c == right_diag) {
                    out << "|\\";
                }
                else {
                    out << "| ";
                }
            }
        }
        out << "\n";
        left_diag -= 1;
        if(left_diag < 0) {
            left_diag = 7;
        }
        right_diag += 1;
        if(right_diag > 7) {
            right_diag = 0;
        }
    }
}


