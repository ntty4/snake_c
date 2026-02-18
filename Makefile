CC = gcc
CLFAGS = -Wall -Werror -Wextra -pedantic

SRC = src/game_exit.c src/key_input.c src/logic.c src/output_screen.c src/terminal.c src/main.c
OUT = bin/main

make:
	@ $(CC) $(SRC) $(FLAGS) -o $(OUT)
