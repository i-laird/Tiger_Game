#include "man_move_helper_functions.h"

bool operator<(const Token_t & a, const Token_t & b) {
    return a.location < b.location;
}


bool secure_from_above(const State& s, int back_row, const Board& brd) {
    bool secure = true;
    set<Point_t> men_positions;
    for(auto i = s.begin(); i != s.end(); ++i) {
        if(i->color == BLUE) {
            men_positions.insert(i->location);
        }
    }
    Token_t tiger;
    tiger.color = RED;
    set<Point_t> tiger_capture_pos;
    // find all potential positions from which tiger may capture
    for(auto i = men_positions.begin(); i != men_positions.end(); ++i){
        tiger.location = *i;
        tiger.location.row -= 1;
        // ensure not checking under back row
        while(tiger.location.row >= back_row){
            --tiger.location.row;
        }
        if(men_positions.find(tiger.location) == men_positions.end()){
            tiger_capture_pos.insert(tiger.location);
        }
        tiger.location.col -= 1;
        if(men_positions.find(tiger.location) == men_positions.end()) {
            tiger_capture_pos.insert(tiger.location);
        }
        tiger.location.col += 2;
        if(men_positions.find(tiger.location) == men_positions.end()) {
            tiger_capture_pos.insert(tiger.location);
        }
    }

    State temp = s;
    // try each tiger potential capture
    for(auto i = tiger_capture_pos.begin(); i != tiger_capture_pos.end(); ++i) {
        tiger.location = *i;
        temp[0] = tiger;
        if(tiger_can_jump(temp, brd) > 0) {
            secure = false;
        }
    }

    if(!secure) {
        State temporary;
        for(auto i = tiger_capture_pos.begin(); i != tiger_capture_pos.end(); ++i) {
            Token_t x;
            x.color = RED;
            x.location = *i;
            temporary.push_back(x);
        }
        cout << "THIS IS NOT SAFE:";
        p_board(cout,s);
        p_board(cout,temporary);
    }

    return secure;
}


Men_Move_F::Men_Move_F(Board& b, int br){
    this->brd = b;
    this->back_row = br;
}


int tiger_can_jump(const State& s, const Board& brd){
    assert(s[0].color == RED);
    int jump = 0;
    // record how many potential jumps are valid moves
    Move_t tiger_jump;
    tiger_jump.token = s[0];
    int sign[3] = {-1,0,1};

    for(int i = 0; i < 9; ++i) {
        tiger_jump.destination = s[0].location;
        tiger_jump.destination.row += 2 * sign[i / 3];
        tiger_jump.destination.col += 2 * sign[i % 3];
        if(brd.isValid(s, tiger_jump)) {
            ++jump;
        }
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
    for(auto i = to_move.begin(); i != to_move.end(); ++i) {
        // create forward move
        new_move.destination = s[*i].location;
        new_move.destination.row -= 1;
        new_move.token = s[*i];
        // if new space is valid and not too far forward
        if(new_move.destination.row >= back_row - 2 && brd.isValid(s, new_move)){
            // record previous location
            Point_t prev_loc = s[*i].location;
            // if tiger cannot jump after move, add to suggested moves
            s[*i].location = new_move.destination;
            if(!tiger_can_jump(s, brd)){
                suggested_moves->push_back(make_pair(*i, new_move));
            }
            // undo move
            s[*i].location = prev_loc;
        }

    }
    return suggested_moves;
}


Men_Move_FLR::Men_Move_FLR(Board& b, int br){
    this->brd = b;
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

    Tiger_Move tiger_mover(brd);// to test tiger moves
    vector<pair<int, Move_t> >* suggested_moves = new vector<pair<int, Move_t> >;
    // for each man that might be moved
    Move_t flr_moves[3];
    for(auto i = to_move.begin(); i != to_move.end(); ++i) {
        // create moves
        for(int j = 0; j < 3; ++j) {
            flr_moves[j].destination = s[*i].location;
            flr_moves[j].token = s[*i];
        }
        flr_moves[0].destination.row -= 1;
        flr_moves[1].destination.col -= 1;
        flr_moves[2].destination.col += 1;
        // for each move
        for(int j = 0; j < 3; ++j) {
            Move_t m = flr_moves[j];
            // if new space is valid and not too far forward, left or right
            if(m.destination.row >= back_row - 2 && brd.isValid(s, m)){
                if(m.destination.col >= left_col && m.destination.col <= right_col) {
                    // record previous location
                    Point_t prev_loc = s[*i].location;
                    // if tiger cannot jump in 1 move or get behind the token
                    // in 2 moves, add to suggested moves
                    s[*i].location = m.destination;
                    if(!tiger_can_jump(s, brd)){
                        // tiger moves
                        vector<pair<int, Move_t> >* t_moves;
                        t_moves = tiger_mover(s, list<int>(1,0));
                        bool behind_token = false;
                        bool jump_two_in_two = false;
                        Point_t tiger_loc = s[0].location;
                        // test each move to see if tiger can get behind man
                        for(auto k = t_moves->begin(); k != t_moves->end(); ++k){
                            s[0].location = k->second.destination;
                            if(s[0].location.row > s[*i].location.row){
                                behind_token = true;
                            }
                            if(tiger_can_jump(s, brd) >= 2) {
                                jump_two_in_two = true;
                            }
                            s[0].location = tiger_loc;
                        }
                        if(!behind_token && !jump_two_in_two) {
                            suggested_moves->push_back(make_pair(*i, m));
                        }
                    }
                    // undo move
                    s[*i].location = prev_loc;
                }
            }
        }
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
        t_move.destination = s[0].location;
        // try moving and try jumping
        for(int j = 0; j < 2; ++j){
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

