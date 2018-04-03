#include "Board.h"


Board::Board() {
    // initialize adjacencies board
    adjacencies = Board_Adj_Mat(13, vector<vector<vector<bool> > >(9,
                            vector<vector<bool>>(13, vector<bool>(9, false))));
    // record horizontal adjacencies
    for(int r = 4; r < 13; ++r) {
        for(int c = 0; c + 1 < 9; ++c) {
            adjacencies[r][c][r][c+1] = adjacencies[r][c+1][r][c] = true;
        }
    }
    // record vertical adjacencies
    for(int r = 4; r + 1 < 13; ++r) {
        for(int c = 0; c < 0; ++ c) {
            adjacencies[r][c][r+1][c] = adjacencies[r+1][c][r][c] = true;
        }
    }
    // record diagonal edges on square portion of board
    int l_col = 4, r_col = 4;
    for(int row = 0; row + 1 < 9; ++row) {
        // set left diagonal
        adjacencies[row][l_col][row + 1][l_col - 1] = true;
        adjacencies[row + 1][l_col - 1][row][l_col] = true;
        // set right diagonal
        adjacencies[row][r_col][row + 1][r_col + 1] = true;
        adjacencies[row + 1][r_col + 1][row][r_col] = true;
        // move forward columns
        l_col -= 1;
        r_col += 1;
        if(l_col - 1 < 0) {
            l_col = 8;
        }
        if(r_col + 1 > 8) {
            r_col = 0;
        }
    }

    // top row of tiger cage to second
    adjacencies[0][4][1][3] = adjacencies[1][3][0][4] = true;
    adjacencies[0][4][1][5] = adjacencies[1][5][0][4] = true;
    // second row of tiger cage to third
    adjacencies[1][3][2][2] = adjacencies[2][2][1][3] = true;
    adjacencies[1][3][2][4] = adjacencies[2][4][1][3] = true;
    adjacencies[1][5][2][6] = adjacencies[2][6][1][5] = true;
    adjacencies[1][5][2][4] = adjacencies[2][4][1][5] = true;
    // third row of tiger cage to fourth
    adjacencies[2][2][3][3] = adjacencies[3][3][2][2] = true;
    adjacencies[2][4][3][3] = adjacencies[3][3][2][4] = true;
    adjacencies[2][4][3][5] = adjacencies[3][4][2][4] = true;
    adjacencies[2][6][3][5] = adjacencies[5][3][2][6] = true;
    // fourth row of tiger cage to fifth
    adjacencies[3][3][4][4] = adjacencies[4][4][3][3] = true;
    adjacencies[3][5][4][4] = adjacencies[3][5][4][4] = true;
}

bool Board::isValid(const State& s, const Move_t& m) const {
    Point_t to, from;
    to = m.destination;
    from = m.token.location;
    bool valid = true;
    // check if to in bounds
    if(to.row < 0 || to.row >= 13 || to.col < 0 || to.col >= 9) {
        valid = false;
    }

    // check if to occupied and ensure from is valid
    set<Point_t> occupied;
    for(auto i = s.begin(); i != s.end(); ++i) {
        occupied.insert(i->location);
    }
    if(occupied.find(from) == occupied.end() ||
                                         occupied.find(to) != occupied.end()) {
        valid = false;
    }

    // make sure they are connected
    if(m.token.color == BLUE && valid) {
        return adjacencies[from.row][from.col][to.row][to.col];
    }
    else if(valid){
        // if not a jump, return whether or not positions are connected
        int col_diff = abs(to.col - from.col);
        int row_diff = abs(to.row - from.row);
        int max_diff = max(col_diff, row_diff);
        int min_diff = min(col_diff, row_diff);
        if(max_diff != 2 || (min_diff != 0 && min_diff != 2) ){
            valid = adjacencies[from.row][from.col][to.row][to.col];
        }
        else {
            Point_t mid = from;
            mid.col += (to.col - from.col) / 2;
            mid.row += (to.row - from.row) / 2;
            // if nothing to jump, not valid
            if(occupied.find(mid) == occupied.end()) {
                valid = false;
            } // make sure jump is valid
            else{
                if(!adjacencies[from.row][from.col][mid.row][mid.col]) {
                    valid = false;
                }
                if(!adjacencies[mid.row][mid.col][to.row][to.col]) {
                    valid = false;
                }
            }
        }
    }
    return valid;
}
