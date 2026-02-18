#ifndef KEY_INPUT_H
#define KEY_INPUT_H


#include "game_var.h"
#define CNTRL_KEY(x) ((x) & 17)

enum commands {
    UP = 1000,
    DOWN,
    LEFT,
    RIGHT
};


int terminalInput(struct game *game);
void parseInput(struct game *game);
void updateMove(struct game *game);


#endif
