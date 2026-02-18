#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

void terminalCooked(void);
void terminalRaw(void);
void getTerminalSize(int *x, int *y);

#endif
