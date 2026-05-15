
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 21
#define HEIGHT 21
#define WALL '#'
#define PATH ' '
#define PLAYER 'P'
#define COIN 'O'
#define EXIT 'E'

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position pos;
    int collected;
    int total;
    int active;
} GameState;

char maze[HEIGHT][WIDTH];
GameState game;
int exitX, exitY;

// Initialize the maze layout
void initMaze() {
    // Simple maze pattern (1=wall, 0=path)
    int template[HEIGHT][WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1},
        {1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
        {1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1},
        {1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1},
        {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1},
        {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    
    // Convert template to maze and place coins & exit
    game.total = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (template[i][j] == 1) {
                maze[i][j] = WALL;
            } else {
                maze[i][j] = PATH;
                // Place coins on some paths (not on start)
                if ((i + j) % 7 == 3 && !(i == 1 && j == 1)) {
                    maze[i][j] = COIN;
                    game.total++;
                }
            }
        }
    }
    // Set exit position
    exitX = 19; exitY = 19;
    maze[exitY][exitX] = EXIT;
    
    // Ensure start position is clear
    game.pos.x = 1; game.pos.y = 1;
    maze[1][1] = PLAYER;
    game.collected = 0;
    game.active = 1;
}

// Draw the game screen
void drawGame() {
    system("cls");
    printf("\n========== MAZE ADVENTURE ==========\n");
    printf("Coins: %d / %d\n\n", game.collected, game.total);
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Store original cell if it's player (don't modify actual maze during draw)
            if (game.pos.x == j && game.pos.y == i) {
                printf("%c ", PLAYER);
            } else {
                printf("%c ", maze[i][j]);
            }
        }
        printf("\n");
    }
    printf("\nControls: WASD / Arrow Keys | R to Restart | Q to Quit\n");
}

// Move player
void movePlayer(int dx, int dy) {
    if (!game.active) return;
    
    int newX = game.pos.x + dx;
    int newY = game.pos.y + dy;
    
    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) return;
    
    char target = maze[newY][newX];
    
    if (target == WALL) return;
    
    // Collect coin
    if (target == COIN) {
        game.collected++;
        maze[newY][newX] = PATH;
    }
    
    // Move player
    game.pos.x = newX;
    game.pos.y = newY;
    
    // Check win condition
    if (newX == exitX && newY == exitY && game.collected == game.total) {
        game.active = 0;
        drawGame();
        printf("\n🎉 CONGRATULATIONS! YOU ESCAPED! 🎉\n");
        printf("Press R to play again or Q to quit.\n");
    }
}

// Reset game state
void resetGame() {
    initMaze();
    drawGame();
}

int main() {
    system("chcp 65001 > nul"); // Support UTF-8 characters
    srand(time(NULL));
    
    initMaze();
    drawGame();
    
    char input;
    while (1) {
        if (kbhit()) {
            input = getch();
            switch (input) {
                case 'w': case 'W': case 72: movePlayer(0, -1); break;
                case 's': case 'S': case 80: movePlayer(0, 1); break;
                case 'a': case 'A': case 75: movePlayer(-1, 0); break;
                case 'd': case 'D': case 77: movePlayer(1, 0); break;
                case 'r': case 'R': resetGame(); break;
                case 'q': case 'Q': printf("\nThanks for playing!\n"); return 0;
            }
            drawGame();
        }
        Sleep(50);
    }
    return 0;
}
