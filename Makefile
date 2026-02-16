CC = gcc
CLFAGS = -Wall -Werror -Wextra -pedantic

SRC = main.c
OUT = main

make:
	@ $(CC) $(SRC) $(FLAGS) -o $(OUT)
