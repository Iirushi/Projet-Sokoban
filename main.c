#include "sokoban.h"
#include <termios.h>
#include <unistd.h>

static struct termios old, current;


void initTermios(int echo) 
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    if (echo) {
        current.c_lflag |= ECHO;
    } else {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
}


void resetTermios(void) 
{
    tcsetattr(0, TCSANOW, &old);
}


char getch() 
{
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();
    return ch;
}

void initGame(Game *game) {
    char initialMap[ROWS][COLS] = {
        "########",
        "#@     #",
        "#  $   #",
        "#  $   #",
        "#  .   #",
        "#  .   #",
        "########"
    };

    game->rows = ROWS;
    game->cols = COLS;
    game->moves = 0;

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            game->map[i][j] = initialMap[i][j];
            if (game->map[i][j] == '@') {
                game->player.row = i;
                game->player.col = j;
            }
        }
    }
}

void printGame(const Game *game) {
    system("clear");  
    printf("Mouvements: %d/%d\n", game->moves, MAX_MOVES);
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            printf("%c", game->map[i][j]);
        }
        printf("\n");
    }
    printf("Utilisez ZQSD pour bouger, E pour quitter\n");
}

bool movePlayer(Game *game, char direction) {
    int newRow = game->player.row;
    int newCol = game->player.col;

    switch (direction) {
        case 'z': newRow--; break;
        case 's': newRow++; break;
        case 'q': newCol--; break;
        case 'd': newCol++; break;
        default: return false;
    }

    if (game->map[newRow][newCol] == '#') return false;

    if (game->map[newRow][newCol] == '$' || game->map[newRow][newCol] == '*') {
        int boxNewRow = newRow + (newRow - game->player.row);
        int boxNewCol = newCol + (newCol - game->player.col);

        if (game->map[boxNewRow][boxNewCol] == '#' || 
            game->map[boxNewRow][boxNewCol] == '$' || 
            game->map[boxNewRow][boxNewCol] == '*') return false;

        game->map[boxNewRow][boxNewCol] = (game->map[boxNewRow][boxNewCol] == '.') ? '*' : '$';
    }

    game->map[game->player.row][game->player.col] = (game->map[game->player.row][game->player.col] == '+') ? '.' : ' ';
    game->map[newRow][newCol] = (game->map[newRow][newCol] == '.' || game->map[newRow][newCol] == '*') ? '+' : '@';
    game->player.row = newRow;
    game->player.col = newCol;
    game->moves++;

    return true;
}

bool checkWin(const Game *game) {
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->map[i][j] == '.' || game->map[i][j] == '+') return false;
        }
    }
    return true;
}

int main() {
    Game game;
    char move;

    initGame(&game);

    while (1) {
        printGame(&game);
        if (checkWin(&game)) {
            printf("Félicitations ! Vous avez gagné en %d mouvements !\n", game.moves);
            break;
        }

        if (game.moves >= MAX_MOVES) {
            printf("Vous avez atteint la limite de %d mouvements. Vous avez perdu !\n", MAX_MOVES);
            break;
        }

        move = getch();
        if (move == 'e' || move == 'E') break;
        movePlayer(&game, move);
    }

    return 0;
}