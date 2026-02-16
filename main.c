#include <stdbool.h>
#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

#define CNTRL_KEY(x) ((x) & 17)

char body = '#';
char head = '@';

enum commands {
    UP = 1000,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct coord_snake{
    int x;
    int y;
} coord_snake;

struct game {
    struct termios orig_term;
    struct winsize ws;

    coord_snake buff_snake[4096];

    char last_move;
    char curr_move;

    int x_screen;
    int y_screen;

    int x_fruit;
    int y_fruit;

    int len_snake;
    bool in_game;

}; struct game game;

typedef struct darr {
    int capacity;
    int size;
    char *arr;
} darr;

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

// ******************************
// terminal cookend and raw mode
// ******************************

void terminalCooked(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &game.orig_term) == -1) die("tcsetattr");
}


void terminalRaw(void) {
    if (tcgetattr(STDIN_FILENO, &game.orig_term) == -1) die("tcgetattr");
    atexit(terminalCooked);

    struct termios rawTerm = game.orig_term;
    rawTerm.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawTerm.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    rawTerm.c_cflag |= (CS8);
    rawTerm.c_oflag &= ~(OPOST);

    rawTerm.c_cc[VMIN] = 0;
    rawTerm.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTerm) == -1) die("tcsetattr");
}

void getTerminalSize(int *x, int *y) {
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &game.ws) == -1 || game.ws.ws_col == 0) die("ioctl");
    *x = game.ws.ws_col;
    *y = game.ws.ws_row;
}

// **************
// parse commands
// *************

int terminalInput(void) {
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
            game.in_game = true;
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


void parseInput(void) {
    game.last_move = game.curr_move;
    int cmd = terminalInput();

    switch(cmd) {
        case CNTRL_KEY('q'): exitGame(); break;

        case UP     :   if (game.curr_move != 'D') {game.last_move = game.curr_move; game.curr_move = 'U';} break;
        case DOWN   :   if (game.curr_move != 'U') {game.last_move = game.curr_move; game.curr_move = 'D';} break;

        case LEFT   :   if (game.curr_move != 'R') {game.last_move = game.curr_move; game.curr_move = 'L';} break;
        case RIGHT  :   if (game.curr_move != 'L') {game.last_move = game.curr_move; game.curr_move = 'R';} break;

    }
}


void updateMove(void) {
    switch (game.curr_move) {
        case 'U': if (game.buff_snake[0].y > 0 && game.last_move != 'D')                 game.buff_snake[0].y--; break;
        case 'D': if (game.buff_snake[0].y <= game.y_screen && game.last_move != 'U')     game.buff_snake[0].y++; break;

        case 'L': if (game.buff_snake[0].x > 0)                 game.buff_snake[0].x--; break;
        case 'R': if (game.buff_snake[0].x <= game.x_screen)     game.buff_snake[0].x++; break;
    }
}


//************+
// Game logic
//*************

bool snakeBitesItSelf(void) {
    for(int i = 2; i < game.len_snake; i++) {
        if(game.buff_snake[0].x == game.buff_snake[i].x && game.buff_snake[0].y == game.buff_snake[i].y) {
            return true;
        }
    }
    return false;
}

bool fruitOnSnake(void) {
    for(int i = 0; i < game.len_snake; i++) {
        if (game.x_fruit == game.buff_snake[i].x || game.y_fruit == game.buff_snake[i].y) {
            return false;
        }
    }
    return true;
}

void gameLogic(void) {
    if (snakeBitesItSelf()) {
        exitGame();
    }

    if(game.buff_snake[0].x == game.x_fruit && game.buff_snake[0].y == game.y_fruit) {
        while(!fruitOnSnake()) {
            game.x_fruit = rand() % game.x_screen;
            game.y_fruit = rand() % game.y_screen;
        }
        game.len_snake += 1;
    }

    if (game.len_snake >= 1 && game.in_game) {
        for (int i = game.len_snake; i > 0; i--) {
            game.buff_snake[i] = game.buff_snake[i-1];
        }
    }

    if (game.buff_snake[0].x == 0 || game.buff_snake[0].y == 0 ||
        game.buff_snake[0].x > game.x_screen || game.buff_snake[0].y > game.y_screen) {
            exitGame();
    }
}



// *****************
// print to terminal
// *****************
void darrAppend(darr* frame, char *str, int size) {
    int new_size = frame->size + size;

    char *new_arr = realloc(frame->arr, (new_size + 1) * sizeof(char));
    memcpy(&new_arr[frame->size], str, size);
    new_arr[new_size] = '\0';

    frame->arr = new_arr;
    frame->size = new_size;
}

void darrFree(darr* frame) {
    free(frame->arr);
    frame->arr = NULL;

    frame->capacity = 0;
    frame->size= 0;
}

void writeScrin(void) {
    darr frame = {0, 0, NULL};
    char buff[32];
    int size;

    size = snprintf(buff, 32, "\x1b[%d;%dH*", game.y_fruit, game.x_fruit);
    darrAppend(&frame, buff, size);


    for(int i = 0; i <= game.len_snake; i++) {

        if (i == 1) {
            size = snprintf(buff, 32, "\x1b[%d;%dH%c", game.buff_snake[0].y, game.buff_snake[0].x, head);
            darrAppend(&frame, buff, size);
            continue;
        }

        size = snprintf(buff, 32, "\x1b[%d;%dH%c", game.buff_snake[i].y, game.buff_snake[i].x, body);
        darrAppend(&frame, buff, size);
    }

    size = snprintf(buff, 32, "\x1b[%d;%dH ", game.buff_snake[game.len_snake].y, game.buff_snake[game.len_snake].x);
    darrAppend(&frame, buff, size);

    write(STDOUT_FILENO, frame.arr, frame.size);
    darrFree(&frame);
}


// **********
// main part
// **********

void initGame(void) {
    cleanScreen();
    getTerminalSize(&game.x_screen, &game.y_screen);

    game.buff_snake[0].x = game.buff_snake[1].x = game.x_screen / 2;
    game.buff_snake[0].y = game.buff_snake[1].y = game.y_screen / 2;

    game.x_fruit = game.x_screen & rand();
    game.y_fruit = game.y_screen & rand();

    game.len_snake = 2;
    game.in_game = false;

    write(STDOUT_FILENO, "\x1b[?25l", 6);

}


int main(void) {
    initGame();
    terminalRaw();

    while(1) {
        parseInput();
        updateMove();
        gameLogic();
        writeScrin();
    }
}
