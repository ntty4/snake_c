#include "../header/terminal.h"
#include "../header/game_exit.h"

struct termios orig_term;
struct winsize ws;

void terminalCooked(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term) == -1) die("tcsetattr");
}


void terminalRaw(void) {
    if (tcgetattr(STDIN_FILENO, &orig_term) == -1) die("tcgetattr");
    atexit(terminalCooked);

    struct termios rawTerm = orig_term;
    rawTerm.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawTerm.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    rawTerm.c_cflag |= (CS8);
    rawTerm.c_oflag &= ~(OPOST);

    rawTerm.c_cc[VMIN] = 0;
    rawTerm.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTerm) == -1) die("tcsetattr");
}

void getTerminalSize(int *x, int *y) {
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) die("ioctl");
    *x = ws.ws_col;
    *y = ws.ws_row;
}
