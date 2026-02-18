#include "../header/game_exit.h"
#include "../header/key_input.h"

#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

int terminalInput(struct game *game) {
    int readBytes;
    char c;

    // reads only one byte
    readBytes = read(STDIN_FILENO, &c, 1);

    if (readBytes == -1 && errno != EAGAIN) die("read");
    if (readBytes == 0) return 0;

    if (c == '\x1b') {
        char buff[3];
        if (read(STDIN_FILENO, &buff[0], 1) != 1) die("read");
        if (read(STDIN_FILENO, &buff[1], 1) != 1) die("read");

        if (buff[0] == '[') {
            game->in_game = true;
            switch (buff[1]) {
                case 'A': return UP;
                case 'B': return DOWN;

                case 'C': return RIGHT;
                case 'D': return LEFT;

            }
        }
    }
    return c;
}


void parseInput(struct game *game) {
    game->last_move = game->curr_move;
    int cmd = terminalInput(game);

    switch(cmd) {
        case CNTRL_KEY('q'): exitGame(); break;

        case UP     :   if (game->curr_move != 'D') {game->last_move = game->curr_move; game->curr_move = 'U';} break;
        case DOWN   :   if (game->curr_move != 'U') {game->last_move = game->curr_move; game->curr_move = 'D';} break;

        case LEFT   :   if (game->curr_move != 'R') {game->last_move = game->curr_move; game->curr_move = 'L';} break;
        case RIGHT  :   if (game->curr_move != 'L') {game->last_move = game->curr_move; game->curr_move = 'R';} break;

    }
}


void updateMove(struct game *game) {
    switch (game->curr_move) {
        case 'U': if (game->buff_snake[0].y > 0 )                       game->buff_snake[0].y--; break;
        case 'D': if (game->buff_snake[0].y <= game->size_screen.y)     game->buff_snake[0].y++; break;

        case 'L': if (game->buff_snake[0].x > 0)                        game->buff_snake[0].x--; break;
        case 'R': if (game->buff_snake[0].x <= game->size_screen.x)     game->buff_snake[0].x++; break;
    }
}
