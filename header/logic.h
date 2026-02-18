#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "../header/game_var.h"

bool snakeBitesItSelf(struct game *game);
bool fruitOnSnake(struct game *game);
void gameLogic(struct game *game);

#endif
