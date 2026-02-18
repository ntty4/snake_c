#include "../header/game_var.h"
#include "../header/terminal.h"
#include "../header/key_input.h"
#include "../header/game_exit.h"
#include "../header/output_screen.h"
#include "../header/logic.h"

struct game game;

void initGame() {
    cleanScreen();
    getTerminalSize(&game.size_screen.x, &game.size_screen.y);

    game.buff_snake[0].x = game.buff_snake[1].x = game.size_screen.x / 2;
    game.buff_snake[0].y = game.buff_snake[1].y = game.size_screen.y / 2;

    game.fruit.x = rand() & game.size_screen.x;
    game.fruit.y = rand() & game.size_screen.y;
    game.head = '@';
    game.body= '#';

    game.len_snake = 2;
    game.in_game = false;
    write(STDOUT_FILENO, "\x1b[?25l", 6);

}

int main() {
    initGame();
    terminalRaw();
    char buff[64];
    int len;

    while(1) {
        parseInput(&game);
        updateMove(&game);
        gameLogic(&game);
        writeScrin(&game);
    }
}
