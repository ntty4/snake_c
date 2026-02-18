#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../header/game_var.h"
#include "../header/output_screen.h"

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

void writeScrin(struct game *game) {
    darr frame = {0, 0, NULL};
    char buff[32];
    int size;

    size = snprintf(buff, 32, "\x1b[%d;%dH*", game->fruit.y, game->fruit.x);
    darrAppend(&frame, buff, size);


    for(int i = 0; i <= game->len_snake; i++) {

        if (i == 1) {
            size = snprintf(buff, 32, "\x1b[%d;%dH%c",
                            game->buff_snake[0].y, game->buff_snake[0].x, game->head);
            darrAppend(&frame, buff, size);
            continue;
        }

        size = snprintf(buff, 32, "\x1b[%d;%dH%c",
                        game->buff_snake[i].y, game->buff_snake[i].x, game->body);
        darrAppend(&frame, buff, size);
    }

    size = snprintf(buff, 32, "\x1b[%d;%dH ",
                    game->buff_snake[game->len_snake].y, game->buff_snake[game->len_snake].x);
    darrAppend(&frame, buff, size);

    write(STDOUT_FILENO, frame.arr, frame.size);
    darrFree(&frame);
}
