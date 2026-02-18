#ifndef GAME_VAR_H
#define GAME_VAR_H
#include <stdbool.h>

typedef struct coord{
    int x;
    int y;
} coord;

struct game{

    coord buff_snake[4096];
    coord size_screen;
    coord fruit;

    char body;
    char head;

    char last_move;
    char curr_move;

    int len_snake;
    bool in_game;

};

#endif
