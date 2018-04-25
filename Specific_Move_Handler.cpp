#include "Specific_Move_Handler.h"

bool Specific_Move_Handler::one_col_two_back() {
    /// verify that in this position
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    bool in_pos = false;
    int back_col = -1;
    // in position if every column except for one is in back_row - 3 and
    // back_row - 2, and that other column is in back_row and back_row - 1
    // and the tiger is in (back_row - 3, back_col)
    if(row_to_col[back_row].size() == 1 &&
       row_to_col[back_row - 1].size() == 1 &&
       row_to_col[back_row - 2].size() == NUM_COL - 1 &&
       row_to_col[back_row - 3].size() == NUM_COL - 1 && front_row >= 4) {

        if(*row_to_col[back_row].begin() == *row_to_col[back_row - 1].begin()) {
            in_pos = true;
            back_col = *row_to_col[back_row].begin();
        }
        if(current->get_tiger().location != make_point(back_row - 3, back_col)){
            in_pos = false;
        }

        for(int r = 0; r < back_row - 1 && in_pos; ++r) {
            if(row_to_col[r].find(back_col) != row_to_col[r].end()) {
                in_pos = false;
            }
        }
   }
   // if back col has an off move, not in position
    if(back_col == off_move.destination.col) {
        in_pos = false;
    }

   if(in_pos) {
        int help_col = back_col - 1;
        if(back_col < 4) {
            help_col = back_col + 1;
        }
        Point_t from, to;
        // move under tiger
        from = make_point(back_row - 2, help_col);
        to = make_point(back_row - 2, back_col);
        response.push(make_move(make_man(from),to));
        // move middle guy in column now holding three to col where higher guy
        // shifted from
        from = make_point(back_row - 1, back_col);
        to = make_point(back_row - 1, help_col);
        response.push(make_move(make_man(from),to));
        // move that guy up now
        /**from = make_point(back_row, help_col);
        to = make_point(back_row - 1, help_col);
        response.push(make_move(make_man(from),to));*/
   }

   return in_pos;
}


bool Specific_Move_Handler::three_by_diag() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = false;
    int left_col = -1;
    // if at tiger cage or not secure, return false
    if(front_row <= 4 || //(row_to_col[front_row].size() < 4 &&
                          //front_row > 6) || (front_row <= 6 &&
                          !secure(current, g)){//)) {
        return false;
    }

    int front_row_diag_cols[2];

    // compute diagonals
    if(front_row <= 8) {
        front_row_diag_cols[0] = front_row;
    }
    else {
        front_row_diag_cols[0] = (front_row + 1) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    Point_t tig_pos = current->get_tiger().location;
    for(int j = 0; j < 2 && left_col < 0; ++j) {
        in_pos = true;
        int c = front_row_diag_cols[j];
        // if tiger to left or right of diagonal, then not in position
        if(abs(tig_pos - make_point(front_row, c)) == make_point(0,1)) {
            in_pos = false;
        }
        // if tiger on the diagonal, not in position
        if(tig_pos == make_point(front_row, c)) {
            in_pos = false;
        }
        if((front_row < 8 && c < 4) || (front_row >= 8 && c >= 4)) {
            c -= 2;
        }
        // c, c + 1, c+ 2 should be three consecutive columns
        // with the diagonal intersecting the front_row at
        // either c or c + 1, and continues down into c
        // if off_col in [c, c+2] then not in pos
        if(c <= off_move.destination.col && c + 2 >= off_move.destination.col) {
            in_pos = false;
        }
        for(int i = 0; i < 3 && in_pos; ++i) {
            if(c + i < 0 || c + i >= NUM_COL) {
                continue;
            }
            if(current->rows_in_col(c + i).size() != 2) {
                in_pos = false;
            }
            if(current->rows_in_col(c + i).find(back_row) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
            if(current->rows_in_col(c + i).find(back_row - 1) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
        }
        if(in_pos) {
            left_col = c;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int diag_col, lim_col, mid_col;
        if(left_col == front_row_diag_cols[0] ||
           left_col == front_row_diag_cols[1]) {
            diag_col = left_col;
            lim_col = left_col + 2;
        }
        else {
            lim_col = left_col;
            diag_col = left_col + 2;
        }
        mid_col = (lim_col + diag_col) / 2;
        Point_t from, to;
        if(tig_pos.col == diag_col) {
            from = make_point(back_row - 1, mid_col);
            to = make_point(back_row - 2, diag_col);
            this->response.push(make_move(make_man(from),to));
            from = make_point(back_row - 1, diag_col);
            to = make_point(back_row - 1, mid_col);
        }
        else {
            from = make_point(back_row - 1, diag_col);
            to = make_point(back_row - 2, diag_col);
            if(abs(tig_pos.col - diag_col) > 1) {
                this->response.push(make_move(make_man(from),to));
                from = make_point(back_row, diag_col);
                to = make_point(back_row - 1, diag_col);
            }
        }
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::near_top_finish_three_by_diag() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = false;
    int left_col = -1;
    // if at tiger cage or not near front return false
    if(front_row <= 4){// || (front_row > 6)) {
        return false;
    }

    int front_row_diag_cols[2];
    // compute diagonals
    if(front_row <= 8) {
        front_row_diag_cols[0] = front_row;
    }
    else {
        front_row_diag_cols[0] = (front_row + 1) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    Point_t tig_pos = current->get_tiger().location;
    for(int j = 0; j < 2 && left_col < 0; ++j) {
        in_pos = true;
        int c = front_row_diag_cols[j];
        // if haven't started a three by diag special move, not in position
        if(!current->is_occupied(make_point(front_row, c)) || tig_pos == make_point(front_row,c)){
            in_pos = false;
        }
        // c, c + 1, c+ 2 should be three consecutive columns
        // with the diagonal intersecting the front_row at
        // either c or c + 1, and continues down into c
        // if any of those has an off move, then not in position
        if(c <= off_move.destination.col && c + 2 >= off_move.destination.col) {
            in_pos = false;
        }
        for(int i = 0; i < 3 && in_pos; ++i) {
            if(c + i < 0 || c + i >= NUM_COL) {
                continue;
            }
            if(current->rows_in_col(c + i).size() != 2) {
                in_pos = false;
            }
            // if don't have back row filled in each column, not in pos
            if(current->rows_in_col(c + i).find(back_row) ==
               current->rows_in_col(c + i).end()) {
                in_pos = false;
            }
            // if dont have row above back row filled in each column except
            // that with the diagonal, not in pos
            if(c + i != front_row_diag_cols[j]) {
                if (current->rows_in_col(c + i).find(back_row - 1) ==
                    current->rows_in_col(c + i).end()) {
                    in_pos = false;
                }
            }
        }
        if(in_pos) {
            left_col = c;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int diag_col;// lim_col;//, mid_col;
        if(left_col == front_row_diag_cols[0] ||
           left_col == front_row_diag_cols[1]) {
            diag_col = left_col;
           // lim_col = left_col + 2;
        }
        else {
           // lim_col = left_col;
            diag_col = left_col + 2;
        }
        //mid_col = (lim_col + diag_col) / 2;
        Point_t from, to;
        from = make_point(back_row, diag_col);
        to = make_point(back_row - 1, diag_col);
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::lagging_col() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    bool in_pos = true;
    int lag_col = -1;
    // if at tiger cage return false
    if(front_row <= 4) {
        return false;
    }
    int front_row_diag_cols[2];

    // compute diagonals of row above front row
    if(front_row - 1 <= 8) {
        front_row_diag_cols[0] = front_row - 1;
    }
    else {
        front_row_diag_cols[0] = (front_row) % NUM_COL;
    }
    front_row_diag_cols[1] = NUM_COL - 1 - front_row_diag_cols[0];

    // if can't have a lagging column by the number of men in each column,
    // not in position
    if(row_to_col[front_row].size() != NUM_COL - 1 ||
       row_to_col[front_row+1].size() != NUM_COL ||
       row_to_col[front_row + 2].size() != 1) {
        in_pos = false;
   }
   else {
        lag_col = *row_to_col[front_row + 2].begin();
        if(row_to_col[front_row].find(lag_col) != row_to_col[front_row].end()) {
            in_pos = false;
        }
        // if lagging col has an off move, not in position
        if(lag_col == off_move.destination.col) {
            in_pos = false;
        }
   }


   // if the lagging column is the middle on the lower half of the board
   // or one of the edges on the upper half, then it is fine to lag
   // and we do not need to do any special moves
   if(front_row >= 8 && lag_col == (NUM_COL - 1) / 2 ) {
       in_pos = false;
   }
   else if(front_row < 8 && (lag_col == 0 || lag_col == NUM_COL - 1)) {
       in_pos = false;
   }



   // if tiger not suppressing lagging column, i.e. not
   // right above it/able to get righ above it, then not lagging
   Token_t tiger = current->get_tiger();
   Point_t tig_pos = tiger.location;
   if(tig_pos != make_point(front_row, lag_col) &&
      !g->isValidMove(*current, make_move(tiger, make_point(front_row, lag_col)))){
        in_pos = false;
   }

   bool protect_diag_from_back = false; // true if it will be necessary
   // to protect from a diagonal attack when switching the lagging col
   // to an appropriate position
   if(lag_col == front_row_diag_cols[0] || lag_col == front_row_diag_cols[1]) {
        // on row 7 don't need to worry about the diagonals for this move
        if(front_row != 8 && front_row != 7) {
            protect_diag_from_back = true;
        }
    }

    // if protecting a diagonal, need the tiger in line with the men
    // else, need room below him to switch into
    if(protect_diag_from_back) {
        if(tig_pos.row != front_row) {
            in_pos = false;
        }
    }
    else {
        if(tig_pos.row != front_row - 1) {
            in_pos = false;
        }
    }

    if(in_pos) {
        this->response = queue<Move_t>();
        int col_in,/* col_2_in,*/ col_3_in;
        if((lag_col < 4 && front_row >= 8) || (lag_col >= 4 && front_row < 8)) {
            col_in = min(tig_pos.col + 1, NUM_COL - 1);
           // col_2_in = min(tig_pos.col + 2, NUM_COL - 1);
            col_3_in = min(tig_pos.col + 3, NUM_COL - 1);
        }
        else {
            col_in = max(0, tig_pos.col - 1);
           // col_2_in = max(0, tig_pos.col - 2);
            col_3_in = max(0, tig_pos.col - 3);
        }

        /*
        if(col_3_in == col_2_in && protect_diag_from_back) {
            return false;
        }
        */

        int tiger_col = tig_pos.col;
        Point_t from, to;
        if(protect_diag_from_back) {
            from = make_point(front_row + 1, col_3_in);
            to = make_point(front_row + 2, col_3_in);
            this->response.push(make_move(make_man(from),to));
        }
        // switch with below tiger
        from = make_point(front_row, col_in);
        to = make_point(front_row, tiger_col);
        this->response.push(make_move(make_man(from),to));
        from = make_point(front_row + 2, tiger_col);
        to = make_point(front_row + 2, col_in);
        this->response.push(make_move(make_man(from),to));
    }

    return in_pos;
}


bool Specific_Move_Handler::try_switch() {
    set<int> row_to_col[NUM_ROW];
    for(int c = 0; c < NUM_COL; ++c) {
        auto r = current->rows_in_col(c).begin();
        while(r != current->rows_in_col(c).end()) {
            row_to_col[*r].insert(c);
            ++r;
        }
    }
    /// verify that in this position
    Token_t tiger = current->get_tiger();
    bool in_pos = false;

    // if at tiger cage or not enough moved up or,
    // or if tiger not above front row or if front row and back row far apart,
    // not in this position
    if(front_row <= 4 || row_to_col[front_row].size() < NUM_COL - 1 ||
       tiger.location.row != front_row - 1 || (back_row - front_row) > 3) {
        return false;
    }

    int tiger_col = tiger.location.col;

    // if tiger not adjacent to diagonal on front row, then return false
    int diag_threatening = -1;
    Point_t tig_pos = tiger.location;
    int diag;
    if(tig_pos.row + 1 <= 8) {
        diag = tig_pos.row + 1;
        Point_t to = make_point(tig_pos.row + 1, diag);
        if(g->isValidMove(*current, make_move(tiger, to))){
            diag_threatening = diag + 1;
        }
        else {
            diag = NUM_COL - 1 - (tig_pos.row + 1);
            Point_t to = make_point(tig_pos.row + 1, diag);
            if(g->isValidMove(*current, make_move(tiger, to))){
                diag_threatening = diag - 1;
            }
        }
    }
    else {
        diag = (tig_pos.row + 2) % NUM_COL;
        Point_t to = make_point(tig_pos.row + 1, diag);
        if(g->isValidMove(*current, make_move(tiger, to))){
            diag_threatening = diag + 1;
        }
        else {
            diag = NUM_COL - 1 - (tig_pos.row + 2) % NUM_COL;
            Point_t to = make_point(tig_pos.row + 1, diag);
            if(g->isValidMove(*current, make_move(tiger, to))){
                diag_threatening = diag - 1;
            }
        }
    }

    // if no help col, return false
    if(diag_threatening < 0 || diag_threatening >= NUM_COL) {
        return false;
    }
    // if either column has an off move, not in a special move
    if(diag_threatening == off_move.destination.col || tiger_col == off_move.destination.col) {
        return false;
    }

    // else, perform the switch on the help_col
    in_pos = true;
    Point_t from, to;
    // move under tiger
    from = make_point(front_row, diag_threatening);
    to = make_point(front_row, tiger_col);
    response.push(make_move(make_man(from),to));
    // move guy protecting diag up
    from = make_point(front_row + 2, diag_threatening + (diag_threatening - tiger_col));
    to = make_point(front_row + 1, diag_threatening + (diag_threatening - tiger_col));
    response.push(make_move(make_man(from),to));
    // protect him
    from = make_point(front_row + 2, tiger_col);
    to = make_point(front_row + 2, diag_threatening);
    response.push(make_move(make_man(from),to));

    return in_pos;
}


Specific_Move_Handler::Specific_Move_Handler(Unordered_State* st, GameRunner* g) {
    this->current = st;
    this->g = g;
    this->front_row = front_row;
    this->back_row = back_row;
}


bool Specific_Move_Handler::handle_special_case(Move_t off_move) {
    if(front_row <= 4) {
        return false;
    }
    this->off_move = off_move;
    // pretend no off move
    bool undid_off = false;
    if(g->isValidMove(*current, -off_move)) {
        undid_off = true;
        current->do_move(-off_move);
    }

    bool in_specific_case = false;
    in_specific_case = one_col_two_back();
    if(!in_specific_case) {
        in_specific_case = three_by_diag();
    }
    if(!in_specific_case) {
        in_specific_case = this->near_top_finish_three_by_diag();
    }
    if(!in_specific_case) {
        in_specific_case = lagging_col();
    }
    if(!in_specific_case) {
        in_specific_case = try_switch();
    }

    if(undid_off) {
        current->do_move(off_move);
    }

    return in_specific_case;
}


void Specific_Move_Handler::set_front_back_row(int front_row, int back_row) {
    this->front_row = front_row;
    this->back_row = back_row;
}


Move_t Specific_Move_Handler::get_move() {
    Move_t to_do = NULL_MOVE;
    if(response.size() > 0) {
        to_do = response.front();
        response.pop();
    }
    return to_do;
}


void Specific_Move_Handler::clear_moves() {
    this->response = queue<Move_t>();
}

