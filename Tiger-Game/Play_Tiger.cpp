#include <sstream>

using namespace std;

#include "Play_Tiger.h"


void Play_Tiger::print_board(ostream& out) {
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

    for(auto i = game_state.begin(); i != game_state.end(); ++i) {
        if(i->color == RED) {
            pts[i->location] = "T";
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
                out << pts[make_point(r,c)] << " ";
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

void Play_Tiger::play(istream & in) {
    // initialize board
    Token_t tiger, man;
    tiger.location = make_point(2,4);
    tiger.color = RED;
    game_state.push_back(tiger);

    man.color = BLUE;
    for(int c = 0; c < 9; ++c){
        for(int r = 11; r < 13; ++ r) {
            man.location = make_point(r,c);
            game_state.push_back(man);
        }
    }
    print_board();
    bool done = false;
    while(!done) {
        Move_t tiger_move, man_move;
        tiger_move.token = game_state[0];
        string kb;
        do {
            tiger_move.destination = tiger_move.token.location;
            getline(in, kb);
            for(unsigned int i = 0; i < kb.size(); ++i) {
                switch(tolower(kb[i])){
                    case 'w': tiger_move.destination.row -= 1;
                              break;
                    case 'a': tiger_move.destination.col -= 1;
                              break;
                    case 's': tiger_move.destination.row += 1;
                              break;
                    case 'd': tiger_move.destination.col += 1;
                              break;
                    case 'q': done = true;
                              break;
                }
            }
            if(!done && !brd.isValid(game_state, tiger_move)){
                cout << "attempted to move from " << game_state[0].location.row << " " << game_state[0].location.col << "\n";
                cout << " to " << tiger_move.destination.row << " " << tiger_move.destination.col << "\n";
                cout << "<ERROR>: " << kb << " is not a valid move\n";
                cout << "please enter one of the following and ensure the selection\n";
                cout << "is valid given the current board state:\n";
                cout << "w  --  forward\n";
                cout << "a  --  left\n";
                cout << "s  --  down\n";
                cout << "d  --  right\n";
                cout << "wa --  up and left\n";
                cout << "wd --  up and right\n";
                cout << "sa --  down and left\n";
                cout << "sd --  down and right\n";
                cout << endl;
                cout << "Press q to quit\n";
            }
        }while(!done && !brd.isValid(game_state, tiger_move));

        if(!done) {
            game_state[0].location = tiger_move.destination;

            stringstream board_one, board_two;
            print_board(dynamic_cast<ostream&>(board_one));


            man_move = men_response.next_move(game_state);

            bool men_moved = false;
            for(auto i = game_state.begin(); i != game_state.end() && !men_moved; ++i) {
                if(*i == man_move.token) {
                    i->location = man_move.destination;
                }
            }

            print_board(dynamic_cast<ostream&>(board_two));

            string line;
            cout << "   tiger moved                  men moved\n";
            cout << "   -----------                  ---------\n";
            while(getline(board_one, line)) {
                cout << line << "         ";
                getline(board_two, line);
                cout << line << endl;
            }
        }
    }
    cout << "GAME OVER\n";
}
