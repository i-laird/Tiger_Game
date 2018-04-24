#include "general_util.h"

/**********************************            *********************************
*********************************** Point Util *********************************
***********************************            ********************************/

/// Point constructors
Point_t make_point(int row, int col) {
    Point_t pt;
    pt.row = row;
    pt.col = col;
    return pt;
}

Point_t make_point(const pair<int,int>& p) {
    Point_t pt;
    pt.row = p.first;
    pt.col = p.second;
    return pt;
}


/// Point comparators
bool operator<(const Point_t& a, const Point_t& b) {
    bool lessThan = false;
    if(a.row < b.row) {
        lessThan = true;
    }
    else if(a.row == b.row && a.col < b.col) {
        lessThan = true;
    }
    return lessThan;
}


bool operator>(const Point_t& a, const Point_t& b) {
    bool greaterThan = false;
    if(a.row > b.row) {
        greaterThan = true;
    }
    else if(a.row == b.row && a.col > b.col) {
        greaterThan = true;
    }
    return greaterThan;
}


bool operator<=(const Point_t& a, const Point_t& b) {
    bool lessThanOrEqual = false;
    if(a.row < b.row) {
        lessThanOrEqual = true;
    }
    else if(a.row == b.row && a.col <= b.row) {
        lessThanOrEqual = true;
    }
    return lessThanOrEqual;
}


bool operator>=(const Point_t& a, const Point_t &b) {
    bool greaterThanOrEqual = false;
    if(a.row > b.row) {
        greaterThanOrEqual = true;
    }
    else if(a.row == b.row && a.col >= b.col) {
        greaterThanOrEqual = true;
    }
    return greaterThanOrEqual;
}


bool operator==(const Point_t& a, const Point_t& b) {
    return (a.row == b.row && a.col == b.col);
}


bool operator!=(const Point_t& a, const Point_t& b) {
    return (a.row != b.row || a.col != b.col);
}


/// Point operators
Point_t operator+(const Point_t& a, const Point_t& b) {
    Point_t pt(a); // copy of a
    pt += b;
    return pt;
}

void operator+=(Point_t& a, const Point_t& b) {
    a.row += b.row;
    a.col += b.col;
}

Point_t operator-(const Point_t& a, const Point_t& b) {
    Point_t pt(a); // copy of a
    pt -= b;
    return pt;
}

void operator-=(Point_t& a, const Point_t& b) {
    a.row -= b.row;
    a.col -= b.col;
}

Point_t operator*(int scalar, const Point_t& vect) {
    Point_t pt = vect;
    pt.row *= scalar;
    pt.col *= scalar;
    return pt;
}

Point_t operator/(const Point_t& vect, int scalar) {
    Point_t pt = vect;
    pt.row /= scalar;
    pt.col /= scalar;
    return pt;
}

Point_t abs(const Point_t& a) {
    return make_point(abs(a.row), abs(a.col));
}


int one_norm(const Point_t& a) {
    return abs(a.row) + abs(a.col);
}


/**********************************            *********************************
*********************************** Token Util *********************************
***********************************            ********************************/

/// Token Constructors

Token_t make_token(Color_t c, const Point_t& pt) {
    Token_t tn;
    tn.color = c;
    tn.location = pt;
    return tn;
}


Token_t make_man(const Point_t& pt) {
    Token_t tn;
    tn.color = BLUE;
    tn.location = pt;
    return tn;
}


Token_t make_tiger(const Point_t& pt) {
    Token_t tn;
    tn.color = RED;
    tn.location = pt;
    return tn;
}


/// Token Comparators
bool operator<(const Token_t& a, const Token_t& b) {
    return (a.location < b.location);
}


bool operator>(const Token_t& a, const Token_t& b) {
    return (a.location > b.location);
}


bool operator<=(const Token_t& a, const Token_t& b) {
    return (a.location < b.location) || (a == b);
}


bool operator>=(const Token_t& a, const Token_t &b) {
    return (a.location > b.location) || (a == b);
}


bool operator==(const Token_t& a, const Token_t& b) {
    return (a.location == b.location) && (a.color == b.color);
}


bool operator!=(const Token_t& a, const Token_t& b) {
    return (a.location != b.location) || (a.color != b.color);
}


/**********************************            *********************************
*********************************** Move Util  *********************************
***********************************            ********************************/

/// Move Constructors
Move_t make_move(const Token_t& tn, const Point_t& dest) {
    Move_t mv;
    mv.token = tn;
    mv.destination = dest;
    return mv;
}


Move_t make_move_in_direction(const Token_t& tn, const Point_t& direc) {
    Move_t mv;
    mv.token = tn;
    mv.destination = tn.location + direc;
    return mv;
}

/// Move comparators
bool operator==(const Move_t& a, const Move_t& b){
    return a.token == b.token && a.destination == b.destination;
}


bool operator!=(const Move_t &a, const Move_t& b) {
    return !(a == b);
}


bool operator<(const Move_t& a, const Move_t& b) {
    return (a.token != b.token ? a.token < b.token : a.destination < b.destination);
}


/// Move operators
Move_t operator-(const Move_t& m) {
    Move_t mv = m;
    swap(mv.token.location, mv.destination);
    return mv;
}


void operator+=(Move_t &a, const Move_t& b) {
    a.destination += b.destination;
    a.token.location += b.token.location;
}


Move_t operator+(const Move_t& a, const Move_t& b) {
    Move_t m = a;
    m += b;
    return m;
}


int one_norm(const Move_t & m) {
    return one_norm(m.destination - m.token.location);
}


