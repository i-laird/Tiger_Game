/**
 * FILENAME:           driver.cpp
 * PROJECT:            DEEP BLUE
 * DATE LAST MODIFIED: June 14, 2020
 *
 * copyright 2020
 */

#include "Smart_Mover.h"
#include "testing_util.h"
#include "constants.h"
#include <ctime>
using namespace std;

/**
 * MAIN
 *
 * Runs a simulation of the TIGER GAME. Allows player to decide if
 * AI will be used for Tiger or if the player wants to play as the Tiger.
 * ALso allows game to be watched in realtime or in replay after a game
 * is compeleted. If desired speedrun can be enabled.
 *
 */
int main()
{
    // by default the tiger will not have AI enabled
    bool auto_tiger = false;

    // used to determine if the game will be displayed as it is simulated
    bool speed_run = false;

    int rando_prob = 0;

    // the number of games that will be simulated
    int num_games = 1;

    // if the player wishes to watch the games as they are simulated
    bool watch_games = true;

    time_t max_time = 0;

    int tiger_wins = 0;

    int draws = 0;

    int men_wins = 0;

    vector<time_t> avg_times;

    double avg_moves = 0;

    int max_moves = 0;

    int max_moves_in_men_win = 0;

    // player input for tiger AI choice
    cout << "Auto tiger (y/n)?\n";
    string playerInput;
    cin >> playerInput;

    // evaluates to Y/N
    if(tolower(playerInput[0]) == 'y') {
        auto_tiger = true;

        // only if Tiger AI is enabled does the random prob need to be specified
        // random prob is the probability that the Tiger will make a
        // completely random move. This is useful if you want the Tiger to
        // not be stuck in oscillation cycles
        cout << "random prob?\n";
        cin >> rando_prob;

        rando_prob = max(rando_prob, 0);
        rando_prob = min(rando_prob, 9);
    }

    // if Tiger AI is chosen
    if(auto_tiger) {

        // player input for if speedrun should be chosen
        cout << "Speed Run (y/n)?\n";
        cin >> playerInput;

        if (tolower(playerInput[0]) == 'y') {
            speed_run = true;

            cout << "How many games?\n";
            cin >> num_games;
            num_games = max(num_games, 1);

            cout << "Watch games?(y/n)\n";
            cin >> playerInput;
            if(tolower(playerInput[0]) != 'y') {
                watch_games = false;
            }
        }
    }

    // if the player has chosen to play it lists the controls
    if(!auto_tiger) {
        cout << "   TIGER GAME:    \n\n\n";
        cout << " Use wasd keys to move the tiger, then press Enter.\n";
        cout << " To use diagonals, use two keys. For example: to \n";
        cout << " move down and left, enter 'sd' or 'ds'\n";
        cout << " Enter 'q' to quit\n";
        cout << "\n";
        cout << "Your Turn: \n\n";
    }

    // each loop is a game simulation
    for(int q = 1; q <= num_games; ++q) {

        // contains all of the states that have occurred so far in the game
        // this is only used if the player has set watch_games
        list<Unordered_State> the_game;

        // signifies that the men have won the game
        // i.e. the tiger can no longer move
        bool men_win = false;

        time_t total_men_time = 0;

        // the number of moves that have been made
        int num_moves = 0;

        State gs(19, NULL_TOKEN);

        // print the game number
        printf("Game %d/%d", q, num_games);

        // initialize the tiger
        gs[0] = make_tiger(make_point(2, 4));

        // initilize the men
        int index = 1;
        for (int row = NUM_ROW - 2; row < NUM_ROW; ++row) {
            for (int col = 0; col < NUM_COL; ++col) {
                gs[index] = make_man(make_point(row, col));
                ++index;
            }
        }

        // runs the game simulation
        GameRunner game;

        // controls the men AI
        Men_Mover * men = new Smart_Mover(gs);

        // the current game state
        Unordered_State game_state(gs);

        bool play_game = true;

        while (play_game && num_moves < MAX_TURNS) {

            // only store previous states if the player has option enabled
            if(watch_games) {
                the_game.push_back(game_state);
                if (the_game.size() > HOW_MANY_TO_STORE) {
                    the_game.pop_front();
                }
            }

            Color_t c;
            State cur = game_state;

            // see if either the men of tiger have won the game
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
                getline(cin, playerInput);
            }

            Move_t tiger_move;
            if (!auto_tiger) {
                string kb;
                do {

                    // the number of user input vertical commands
                    int verticalCount   = 0;

                    // the number of user input horizontal commands
                    int horizontalCount = 0;

                    tiger_move = make_move(game_state.get_tiger(), NULL_POINT);
                    tiger_move.destination = tiger_move.token.location;

                    // get the user inputed move for the tiger
                    getline(cin, kb);
                    for (unsigned int i = 0; i < kb.size(); ++i) {

                        if(horizontalCount > 1 || verticalCount > 1){
                            cerr << "ERROR: Invalid Move" << endl;
                            tiger_move = NULL_MOVE;
                        }

                        // loop through ever user inputed key
                        switch (tolower(kb[i])) {
                            case 'w':
                                tiger_move.destination += UP;
                                ++verticalCount;
                                break;
                            case 'a':
                                tiger_move.destination += LEFT;
                                ++horizontalCount;
                                break;
                            case 's':
                                tiger_move.destination += DOWN;
                                ++verticalCount;
                                break;
                            case 'd':
                                tiger_move.destination += RIGHT;
                                ++horizontalCount;
                                break;
                            case 'q':
                                play_game = false;
                                break;
                        }
                    }

                    // if the move entered is not valid
                    if (play_game && !game.isValidMove(game_state, tiger_move)) {

                        cout << "<ERROR>: " << kb << " is not a valid move: ";
                        cout << tiger_move.token.location << " --> " << tiger_move.destination;
                        cout << "\n";
                        cout << "<valid moves>:\n";

                        // get the moves that the tiger can make
                        auto valid_moves = game.validMoves(game_state, game_state.get_tiger());

                        // list these so that the user can choose
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
            }
            // get the AI tiger move if the user is not playing
            else {
                State cur = game_state;
                tiger_move = game.Tiger_Move(cur, rando_prob);
            }

            // perform the tiger move
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
            playerInput = "";
            cout << "game lost... watch game?(y/n)";
            cin >> playerInput;
            playerInput[0] = tolower(playerInput[0]);
            if(playerInput[0] == 'y') {
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
        if(men_win) {
            max_moves_in_men_win = max(num_moves, max_moves_in_men_win);
        }
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
        if(men_wins < num_games) {
            cout << "max num turns during win = " << max_moves_in_men_win << "\n";
        }
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
