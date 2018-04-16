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

   if(in_pos) {
        int help_col = back_col - 1;
        if(back_col < 4) {
            help_col = back_col + 1;
        }
        Point_t from, to;
        from = make_point(back_row - 2, help_col);
        to = make_point(back_row - 2, back_col);
        response.push(make_move(make_man(from),to));
        from = make_point(back_row , back_col);
        to = make_point(back_row, help_col);
        response.push(make_move(make_man(from),to));
        from = make_point(back_row, help_col);
        to = make_point(back_row - 1, help_col);
        response.push(make_move(make_man(from),to));
   }

   return in_pos;
}

Specific_Move_Handler::Specific_Move_Handler(const Unordered_State* st) {
    this->current = st;
    this->front_row = front_row;
    this->back_row = back_row;
}


bool Specific_Move_Handler::handle_special_case() {
    bool in_specific_case = false;
    in_specific_case = one_col_two_back();

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
