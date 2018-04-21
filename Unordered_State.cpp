#include "Unordered_State.h"

/// Unordered_State constructors
Unordered_State::Unordered_State() {
    this->tiger = NULL_TOKEN;
    for(int i = 0; i < NUM_COL; ++i) {
        col_to_rows[i] = set<int>();
    }
}


Unordered_State::Unordered_State(const State& st) {
    for(int i = 0; i < NUM_COL; ++i) {
        col_to_rows[i] = set<int>();
    }
    // look at each token in State, if tiger store in tiger,
    // if a man add its location to the set of men
    for(auto i = st.begin(); i != st.end(); ++i) {
        if(i->color == RED) {
            this->tiger = *i;
        }
        else if(i->color == BLUE) {
            this->col_to_rows[i->location.col].insert(i->location.row);
        }
    }
}


/// Comparators
bool Unordered_State::operator<(const Unordered_State& that) const {
    bool lessThan = false;
    //compare tiger
    if(this->tiger < that.tiger) {
        lessThan = true;
    }// if same tiger, compare men locs
    else if(this->tiger == that.tiger) {
        bool determined = false;
        for(int i = 0 ; i < NUM_COL && !determined; ++i) {
            if(this->col_to_rows[i] < that.col_to_rows[i]) {
                determined = true;
                lessThan = true;
            }
            else if(this->col_to_rows[i] > that.col_to_rows[i]) {
                determined = true;
                lessThan = false;
            }
        }
    }


    return lessThan;
}


bool Unordered_State::operator>(const Unordered_State& that) const {
    return (that < *this && that != *this);
}


bool Unordered_State::operator<=(const Unordered_State& that) const {
    return (*this < that) || (*this == that);
}


bool Unordered_State::operator>=(const Unordered_State& that) const {
    return (*this > that) || (*this == that);
}


bool Unordered_State::operator==(const Unordered_State& that) const {
    bool equals = true;
    if(this->tiger != that.tiger) {
        equals = false;
    }
    else {
        for(int i = 0 ; i < NUM_COL && equals; ++i) {
            if(this->col_to_rows[i] != that.col_to_rows[i]) {
                equals = false;
            }
        }
    }
    return equals;
}


bool Unordered_State::operator!=(const Unordered_State& that) const {
    return !(*this == that);
}


/// Accessor functions
const Token_t& Unordered_State::get_tiger() const {
    return this->tiger;
}


const set<int>& Unordered_State::rows_in_col(int col) const {
    // set col to nearest column in range
    col = max(0, col);
    col = min(col, NUM_COL - 1);

    return this->col_to_rows[col];
}


/// Mutator Functions
void Unordered_State::set_tiger(const Token_t& t) {
    this->tiger = t;
}


void Unordered_State::set_men_locs(const set<Point_t>& locs) {
    for(int i = 0; i < NUM_COL; ++i) {
        col_to_rows[i] = set<int>();
    }
    for(auto i = locs.begin(); i != locs.end(); ++i) {
        if(i->col >= 0 && i->col < NUM_COL) {
            col_to_rows[i->col].insert(i->row);
        }
    }
}


/// operators
bool Unordered_State::do_move(const Move_t& m) {
    bool valid_move = true; // true iff moving a token that
                            // exists to an unoccupied position
    Point_t from = m.token.location; // where going from
    Point_t to = m.destination; // where going to
    // this is a bad move if not in the column range
    if(from.col < 0 || from.col >= NUM_COL || to.col < 0 ||
       from.col >= NUM_COL) {
        valid_move = false;
    }
    // this is a bad move if moving into tiger location
    if(to == this->tiger.location) {
        valid_move = false;
    }

    // moving a man
    if(valid_move && m.token.color == BLUE) {
        // try to erase previous location, if cannot then trying to move
        // a token which does not exist
        if(this->col_to_rows[from.col].erase(from.row) <= 0) {
            valid_move = false;
        } // if did erase, but moving to an occupied position, that is invalid
        else if(!this->col_to_rows[to.col].insert(to.row).second){
            valid_move = false;
            this->col_to_rows[from.col].insert(from.row);
        }
    }// moving a tiger
    else if(valid_move && m.token.color == RED) {
        if(from != this->tiger.location) {
            valid_move = false;
        }
        else if(this->col_to_rows[to.col].find(to.row) == this->col_to_rows[to.col].end()) {
            this->tiger.location = to;
        }
        else {
            valid_move = false;
        }

        // if a jump, remove the man that was jumped
        Point_t jumped_pos = (to + from) / 2;
        if(rows_in_col(jumped_pos.col).find(jumped_pos.row) !=
                                            rows_in_col(jumped_pos.col).end()) {
            col_to_rows[jumped_pos.col].erase(jumped_pos.row);
        }
    }

    return valid_move;
}


 Unordered_State::operator State() const{
    State st;
    st.push_back(this->tiger);
    for(int c = 0; c < NUM_COL; ++c) {
        for(auto r = col_to_rows[c].begin(); r != col_to_rows[c].end(); ++r) {
            st.push_back(make_man(make_point(*r, c)));
        }
    }
    return st;
}


/// functions to convenience getting information
bool Unordered_State::is_occupied(const Point_t& pt) const {
    bool occupied = false;
    // if column in range
    if(pt.col >= 0 && pt.col < NUM_COL) {
        if(pt == this->tiger.location) {
            occupied = true;
        }
        else if(col_to_rows[pt.col].find(pt.row) != col_to_rows[pt.col].end()) {
            occupied = true;
        }
    }

    return occupied;
}


