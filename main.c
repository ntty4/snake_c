#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

struct game {
    struct termios orig_term;
};

struct game game;


void printError(char *s) {
    perror(s);
    exit(1);
}


void terminalCooked(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &game.orig_term) == -1) printError("tcsetattr");
}


void terminalRaw(void) {
    if (tcgetattr(STDIN_FILENO, &game.orig_term) == -1) printError("tcgetattr");
    atexit(terminalCooked);

    struct termios rawTerm = game.orig_term;
    rawTerm.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawTerm.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    rawTerm.c_cflag |= (CS8);
    rawTerm.c_oflag &= ~(OPOST);

    rawTerm.c_cc[VMIN] = 0;
    rawTerm.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTerm) == -1) printError("tcsetattr");
}

// parse commands

int takeCommands(void){
    int command;
    if (read(STDIN_FILENO, &command, 1) == -1 && errno != EAGAIN) printError("read");
}



int main(void) {
    terminalRaw();
}
