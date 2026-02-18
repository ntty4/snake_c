#include "../header/game_exit.h"

void cleanScreen(void) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}

void exitGame(void) {
    cleanScreen();
    exit(0);
}


void die(char *s) {
    cleanScreen();
    perror(s);
    exit(1);
}
