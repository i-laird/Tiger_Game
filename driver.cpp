#include "Smart_Mover.h"
#include "testing_util.h"
#include "Scored_MMover.h"
#include "Scored_MMovers/Upward.h"
#include "Scored_MMovers/Inward.h"
#include "Scored_MMovers/AvoidCapture.h"
#include "Scored_MMovers/MarkTiger.h"
#include <iomanip>
#include <ctime>
using namespace std;

const int MAX_TURNS = 1000;
const int HOW_MANY_TO_STORE = 50;

int main()
{
    bool auto_tiger = false;
    bool speed_run = false;
    int rando_prob = 0;
    cout << "Auto tiger (y/n)?\n";
    string trash;
    cin >> trash;
    if(tolower(trash[0]) == 'y') {
        auto_tiger = true;
        cout << "random prob?\n";
        cin >> rando_prob;
        rando_prob = max(rando_prob, 0);
        rando_prob = min(rando_prob, 9);
    }

    int num_games = 1;

    bool watch_games = true;
    if(auto_tiger) {
        cout << "Speed Run (y/n)?\n";
        cin >> trash;
        if (tolower(trash[0]) == 'y') {
            speed_run = true;
            cout << "How many games?\n";
            cin >> num_games;
            num_games = max(num_games, 1);
            cout << "Watch games?(y/n)\n";
            cin >> trash;
            if(tolower(trash[0]) != 'y') {
                watch_games = false;
            }
        }
    }

    if(!auto_tiger) {
        cout << "   TIGER GAME:    \n\n\n";
        cout << " Use wasd keys to move the tiger, then press Enter.\n";
        cout << " To use diagonals, use two keys. For example: to \n";
        cout << " move down and left, enter 'sd' or 'ds'\n";
        cout << " Enter 'q' to quit\n";
        cout << "\n";
        cout << "Your Turn: \n\n";
    }

    time_t max_time = 0;
    int tiger_wins = 0;
    int draws = 0;
    int men_wins = 0;
    vector<time_t> avg_times;
    double avg_moves = 0;
    int max_moves = 0;

    for(int q = 0; q < num_games; ++q) {
        if((100 * q) / num_games != (100 * (q - 1)) / num_games) {
            cout << q << " / " << num_games << "...\n";
        }
        list<Unordered_State> the_game;
        bool men_win = false;

        time_t total_men_time = 0;
        int num_moves = 0;
        State gs(19, NULL_TOKEN);
        gs[0] = make_tiger(make_point(2, 4));
        int index = 1;
        for (int row = NUM_ROW - 2; row < NUM_ROW; ++row) {
            for (int col = 0; col < NUM_COL; ++col) {
                gs[index] = make_man(make_point(row, col));
                ++index;
            }
        }
        GameRunner game;
        //Men_Mover* men = (new Scored_MMover(gs))->addMod(new Upward())->addMod(new Upward())->addMod(new Inward())->addMod(new AvoidCapture())->addMod(new MarkTiger());//new Smart_Mover(gs);
        Men_Mover *men = new Smart_Mover(gs);
        Unordered_State game_state(gs);
        bool play_game = true;
        while (play_game && num_moves < MAX_TURNS) {
            if(watch_games) {
                the_game.push_back(game_state);
                if (the_game.size() > HOW_MANY_TO_STORE) {
                    the_game.pop_front();
                }
            }
            Color_t c;
            State cur = game_state;
            if (game.evaluateWinState(cur, c)) {
                play_game = false;
                if (c == BLUE) {
                    ++men_wins;
                    men_win = true;
                    if(num_games <= 1) {
                        cout << "THE MEN WIN!!!\n";
                    }
                } else if (c == RED) {
                    ++tiger_wins;
                    if(num_games <= 1) {
                        cout << "THE TIGER WINS!\n";
                    }
                }
            }
            if (!play_game && num_games <= 1) {
                p_board(game_state);
                break;
            }

            if (!speed_run) {
                p_board(game_state);
            }
            if (auto_tiger && !speed_run) {
                cout << "Press Enter to continue.\n";
                getline(cin, trash);
            }

            Move_t tiger_move;
            if (!auto_tiger) {
                string kb;
                do {
                    tiger_move = make_move(game_state.get_tiger(), NULL_POINT);
                    tiger_move.destination = tiger_move.token.location;
                    getline(cin, kb);
                    for (unsigned int i = 0; i < kb.size(); ++i) {
                        switch (tolower(kb[i])) {
                            case 'w':
                                tiger_move.destination += UP;
                                break;
                            case 'a':
                                tiger_move.destination += LEFT;
                                break;
                            case 's':
                                tiger_move.destination += DOWN;
                                break;
                            case 'd':
                                tiger_move.destination += RIGHT;
                                break;
                            case 'q':
                                play_game = false;
                                break;
                        }
                    }
                    if (play_game && !game.isValidMove(game_state, tiger_move)) {
                        cout << "<ERROR>: " << kb << " is not a valid move: ";
                        cout << tiger_move.token.location << " --> " << tiger_move.destination;
                        cout << "\n";
                        cout << "<valid moves>:\n";
                        auto valid_moves = game.validMoves(game_state, game_state.get_tiger());
                        for (auto i = 0; i < valid_moves.second.second; ++i) {
                            Point_t to = valid_moves.first[i];
                            string cmd = "";
                            string direc = " (";
                            Point_t diff = to - game_state.get_tiger().location;
                            while (diff.row > 0) {
                                --diff.row;
                                cmd += "w";
                                direc += " UP";
                            }
                            while (diff.row < 0) {
                                ++diff.row;
                                cmd += "s";
                                direc += " DOWN";
                            }
                            while (diff.col < 0) {
                                ++diff.col;
                                cmd += "a";
                                direc += " RIGHT";
                            }
                            while (diff.col > 0) {
                                --diff.col;
                                cmd += "d";
                                direc += " LEFT";
                            }
                            cout << " " << cmd << direc << " to " << to << ")\n";
                        }
                        delete[] valid_moves.first;
                        delete[] valid_moves.second.first;
                    }
                } while (play_game && !game.isValidMove(game_state, tiger_move));
            } else {
                State cur = game_state;
                tiger_move = game.Tiger_Move(cur, rando_prob);
            }

            game_state.do_move(tiger_move);
            time_t start = clock();
            Move_t men_move = men->next_move(game_state);
            time_t end = clock();
            total_men_time += (end - start);
            max_time = max((end - start), max_time);
            ++num_moves;
            if (!speed_run) {
                cout << "men time: " << end - start << endl;
            }
            if (men_move == NULL_MOVE) {
                cout << "<ERROR>: no men move received\n";
                cout << "BOARD STATE\n";
                p_board(game_state);
                if (speed_run) {
                    p_board(game_state);
                    break;
                }
            } else if (!game.isValidMove(game_state, men_move)) {
                cout << "<ERROR>: men move invalid\n";
                cout << "         " << men_move << "\n";
                cout << "BOARD STATE\n";
                p_board(game_state);
                if (speed_run) {
                    p_board(game_state);
                    break;
                }
            }

            game_state.do_move(men_move);
            auto t_moves = game.validMoves(game_state, game_state.get_tiger());

            if (tiger_can_jump(&game_state, &game) && !speed_run) {
                cout << "<ALERT> tiger can jump men\n";
            }
            if (game_state.get_tiger().color != RED && !speed_run) {
                cout << "<ALERT> tiger is not red\n";
            }

            if(t_moves.first) {
                delete [] t_moves.first;
                t_moves.first = nullptr;
            }
            if(t_moves.second.first) {
                delete [] t_moves.second.first;
                t_moves.second.first = nullptr;
            }
        }

        delete men;
        if(!men_win && speed_run && watch_games) {
            trash = "";
            cout << "game lost... watch game?(y/n)";
            cin >> trash;
            trash[0] = tolower(trash[0]);
            if(trash[0] == 'y') {
                cout << "enter a positive number to go forwards, negative to go back, 0 to quit\n";
                cout << "enter any positive number to start.\n";
                bool viewing = true;
                int turn = 0;
                int skip;
                while(viewing) {
                    cin >> skip;
                    if(skip != 0) {
                        turn += skip;
                    }
                    else {
                        viewing = false;
                    }
                    if(viewing) {
                        if(turn < 0) {
                            cout << "at first turn...\n";
                            turn = 0;
                        }
                        else if(turn >= the_game.size()) {
                            cout << "at last turn ...\n";
                            turn = the_game.size() - 1;
                        }
                        else {
                            auto i = the_game.begin();
                            for(int j = 0; j < turn; ++j) {
                                ++i;
                            }
                            p_board(*i);
                        }
                    }
                }
            }
        }

        if(num_moves >= MAX_TURNS) {
            ++draws;
        }

        avg_moves += num_moves;
        max_moves = max(num_moves, max_moves);
        avg_times.push_back(total_men_time / num_moves);
    }

    if(num_games <= 1) {
        cout << "num turns = " << avg_moves << "\n";
        cout << "avg time = " << avg_times[0] << " ms\n";
        cout << "max time = " << max_time << " ms\n";
    }
    else {
        std::sort(avg_times.begin(), avg_times.end());
        time_t avg_avg_time = 0;
        for(auto t : avg_times) {
            avg_avg_time += t;
        }
        avg_avg_time /= avg_times.size();
        cout << "avg num turns = " << avg_moves / num_games << "\n";
        cout << "max num turns = " << max_moves << "\n";
        cout << "median avg time = " << avg_times[avg_times.size() / 2] << " ms\n";
        cout << "avg avg time = " << avg_avg_time << " ms\n";
        cout << "max time = " << max_time << " ms\n";
        cout << "\n";
        cout << "men wins: " << men_wins << "\n";
        cout << "tiger wins: " << tiger_wins << "\n";
        cout << "draws: " << draws << "\n";
    }


    cout << "\nGAME OVER\n";

    return 0;
}
