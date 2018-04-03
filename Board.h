#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <set>
#include <vector>
#include <cmath>

#include "game_runner.h"
#include "function_api.h"

using namespace std;

typedef vector<Token_t> State;
typedef vector<vector<vector<vector<bool> > > > Board_Adj_Mat;

class Board{
    private:
        Board_Adj_Mat adjacencies;
    public:
        Board();
        bool isValid(const State& s, const Move_t& m) const;
};

#endif // BOARD_H_INCLUDED
