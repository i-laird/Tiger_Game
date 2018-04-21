#include "Smart_Mover.h"
#include "testing_util.h"
#include "Scored_MMover.h"
#include "Scored_MMovers/Upward.h"
#include "Scored_MMovers/Inward.h"
#include <iomanip>
using namespace std;

int main()
{
    //play_as_tiger();

    cout << "   TIGER GAME:    \n\n\n";
    cout << " Use wasd keys to move the tiger, then press Enter.\n";
    cout << " To use diagonals, use two keys. For example: to \n";
    cout << " move down and left, enter 'sd' or 'ds'\n";
    cout << " Enter 'q' to quit\n";
    cout << "\n";
    cout << "Your Turn: \n\n";

    State gs(19, NULL_TOKEN);
    gs[0] = make_tiger(make_point(2,4));
    int index = 1;
    for(int row = NUM_ROW - 2; row < NUM_ROW; ++row) {
        for(int col = 0; col < NUM_COL; ++col) {
            gs[index] = make_man(make_point(row,col));
            ++index;
        }
    }
    GameRunner game;
    Men_Mover* men = (new Scored_MMover(gs))->addMod(new Upward())->addMod(new Upward())->addMod(new Inward());//new Smart_Mover(gs);
    Unordered_State game_state(gs);
    bool play_game = true;
    while(play_game) {
        p_board(game_state);

        Move_t tiger_move;
        string kb;
        do {
            tiger_move = make_move(gs[0], NULL_POINT);
            tiger_move.destination = tiger_move.token.location;
            getline(cin, kb);
            for(unsigned int i = 0; i < kb.size(); ++i) {
                switch(tolower(kb[i])){
                    case 'w': tiger_move.destination += UP;
                              break;
                    case 'a': tiger_move.destination += LEFT;
                              break;
                    case 's': tiger_move.destination += DOWN;
                              break;
                    case 'd': tiger_move.destination += RIGHT;
                              break;
                    case 'q': play_game = false;
                              break;
                }
            }
            if(play_game && !game.isValidMove(game_state, tiger_move)){
                cout << "<ERROR>: " << kb << " is not a valid move: ";
                cout << tiger_move.token.location << " --> " << tiger_move.destination;
                cout << "\n";
                cout << "<valid moves>:\n";
                pair<Point_t*, pair<bool*, int>> valid_moves;
                valid_moves = game.validMoves(game_state, gs[0]);
                for(auto i = 0; i < valid_moves.second.second; ++i) {
                    Point_t to = valid_moves.first[i];
                    string cmd = "";
                    string direc = " (";
                    Point_t diff = to - gs[0].location;
                    while(diff.row > 0) {
                        --diff.row;
                        cmd += "w";
                        direc += " UP";
                    }
                    while(diff.row < 0) {
                        ++diff.row;
                        cmd += "s";
                        direc += " DOWN";
                    }
                    while(diff.col < 0) {
                        ++diff.col;
                        cmd += "a";
                        direc += " RIGHT";
                    }
                    while(diff.col > 0) {
                        --diff.col;
                        cmd += "d";
                        direc += " LEFT";
                    }
                    cout << " " << cmd << direc << " to " << to << ")\n";
                }
                delete [] valid_moves.first;
                delete [] valid_moves.second.first;
            }
        }while(play_game && !game.isValidMove(gs, tiger_move));

        gs[0].location = tiger_move.destination;
        game_state.do_move(tiger_move);
        Move_t men_move = men->next_move(tiger_move);
        if(men_move == NULL_MOVE) {
            cout << "<ERROR>: no men move received\n";
        }
        else if(!game.isValidMove(game_state, men_move)) {
            cout << "<ERROR>: men move invalid\n";
            cout << "         " << men_move << "\n";
        }
        // move men
        for(int i = 1; i < 19; ++i) {
            if(gs[i] == men_move.token) {
                gs[i].location = men_move.destination;
                break;
            }
        }
        game_state.do_move(men_move);
        pair<Point_t *, pair<bool*,int>> t_moves;
        t_moves = game.validMoves(game_state, game_state.get_tiger());

        if(tiger_can_jump(&game_state, &game)) {
            cout << "<ALERT> tiger can jump men\n";
        }
        if(game_state.get_tiger().color != RED) {
            cout << "<ALERT> tiger is not red\n";
        }
    }

    delete men;

    cout << "\nGAME OVER\n";
    return 0;
}
