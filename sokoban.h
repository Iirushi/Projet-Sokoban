#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 7
#define COLS 8
#define MAX_MOVES 25

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    char map[ROWS][COLS];
    int rows;
    int cols;
    Position player;
    int moves;
} Game;

void initGame(Game *game);
void printGame(const Game *game);
bool movePlayer(Game *game, char direction);
bool checkWin(const Game *game);

#endif // SOKOBAN_H