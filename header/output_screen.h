#ifndef OUTPUT_SCREEN_H
#define OUTPUT_SCREEN_H

typedef struct darr {
    int capacity;
    int size;
    char *arr;
} darr;

void darrAppend(darr* frame, char *str, int size);
void darrFree(darr* frame);
void writeScrin(struct game *game);

#endif
