#include "../header/game_exit.h"
#include "../header/logic.h"

bool snakeBitesItSelf(struct game *game) {
    for(int i = 2; i < game->len_snake; i++) {
        if(game->buff_snake[0].x == game->buff_snake[i].x && 
            game->buff_snake[0].y == game->buff_snake[i].y) {
            return true;
        }
    }
    return false;
}

bool fruitOnSnake(struct game *game) {
    for(int i = 0; i < game->len_snake; i++) {
        if (game->fruit.x == game->buff_snake[i].x || game->fruit.y == game->buff_snake[i].y) {
            return false;
        }
    }
    return true;
}

void gameLogic(struct game *game) {
    if (snakeBitesItSelf(game)) {
        exitGame();
    }

    if(game->buff_snake[0].x == game->fruit.x && game->buff_snake[0].y == game->fruit.y) {
        while(!fruitOnSnake(game)) {
            game->fruit.x = rand() % game->size_screen.x;
            game->fruit.y = rand() % game->size_screen.y;
        }
        game->len_snake += 1;
    }

    if (game->buff_snake[0].x == 0 || game->buff_snake[0].y == 0 ||
        game->buff_snake[0].x > game->size_screen.x || game->buff_snake[0].y > game->size_screen.y) {
            exitGame();
    }
}
