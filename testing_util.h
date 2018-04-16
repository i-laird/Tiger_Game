#include <iostream>
#include <cassert>
#include <map>
#include "general_util.h"
#include "game_runner.h"
//#include "Men_Mover.h"

using namespace std;

typedef pair<int,int> Hash_val;

#ifndef TESTING_UTIL_H_INCLUDED
#define TESTING_UTIL_H_INCLUDED

ostream& operator<<(ostream& out, const Point_t& pt);

ostream& operator<<(ostream& out, const pair<int,int>& p);

ostream& operator<<(ostream& out, const Move_t& m);

istream& operator>>(istream& in, Point_t& pt);

ostream& operator>>(ostream& out, bool b);

void p_board(const State& s, ostream& out = cout);

template <class T>
ostream& operator<<(ostream& out, const vector<T>& v) {
    for(auto i = v.begin(); i != v.end(); ++i) {
        out << (*i) << " ";
    }
    return out;
}

State inverse_hash(Hash_val h, int back_row, Token_t tiger = NULL_TOKEN);

void play_as_tiger();


#endif // TESTING_UTIL_H_INCLUDED
