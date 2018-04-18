#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <set>
#include <vector>
#include <cmath>

#include "game_runner.h"
#include "function_api.h"

using namespace std;

typedef vector<Token_t> State;
typedef pair<Token_t, set<Point_t>> Orderless_State; // tiger token and men pos
typedef vector<vector<vector<vector<bool> > > > Board_Adj_Mat;

Point_t make_point(int r, int c);

class Board{
    private:
        Board_Adj_Mat adjacencies;
        set<Point_t> tiger_cage;
    public:
        Board();
        bool isVertex(Point_t p) const;
        bool isValid(const State& s, const Move_t& m) const;
        std::vector<Move_t> listPossible(const State& s, const Token_t t);
};

#endif // BOARD_H_INCLUDED
