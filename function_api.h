#ifndef TIGER_GAME_FUNCTION_API_H
#define TIGER_GAME_FUNCTION_API_H

#include <vector>
using namespace std;

enum Color_t {RED, BLUE};

struct Point_t{
    int row, col;
};

struct Token_t{
    Color_t color;
    Point_t location;
};

struct Move_t{
    Token_t token;
    Point_t destination;
};



#endif //TIGER_GAME_FUNCTION_API_H
