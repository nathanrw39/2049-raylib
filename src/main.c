#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// define size of the board
#define SIZE 4
#define TILE_SIZE 100
#define TILE_PADDING 10

// function definitions
void InitBoard(int board[SIZE][SIZE]);
void DrawBoard(int board[SIZE][SIZE]);
void AddTile(int board[SIZE][SIZE]);
bool MoveAndMergeTiles(int board[SIZE][SIZE], int dx, int dy);
bool CanMove(int board[SIZE][SIZE]);

// main function establishing screen
int main(void) {
    const int screenWidth = 500;
    const int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "2048");
    SetTargetFPS(60);

    int board[SIZE][SIZE] = {0};
    InitBoard(board);
    
    // set gameover false
    bool gameOver = false;

    while (!WindowShouldClose()) {
        if (!gameOver) {
            bool moved = false;
    
            // detect user input and move tiles 
            if (IsKeyPressed(KEY_W)) moved = MoveAndMergeTiles(board, 0, -1);
            if (IsKeyPressed(KEY_S)) moved = MoveAndMergeTiles(board, 0, 1);
            if (IsKeyPressed(KEY_A)) moved = MoveAndMergeTiles(board, -1, 0);
            if (IsKeyPressed(KEY_D)) moved = MoveAndMergeTiles(board, 1, 0);
    
            if (moved) AddTile(board);
    
            // check for game over
            if (!CanMove(board)) {
                gameOver = true;
            }
        }
    
        // draw game state
        BeginDrawing();
        ClearBackground(RAYWHITE);
    
        if (gameOver) {
            // draw game over
            DrawText("Game Over!", 150, 200, 40, RED);
            DrawText("Press R to Restart", 120, 250, 20, DARKGRAY);
    
            // restart game when pressing r
            if (IsKeyPressed(KEY_R)) {
                InitBoard(board); 
                gameOver = false;
            }
        } else {
            
            DrawBoard(board);
        }
    
        EndDrawing();
    }
}
// initialize the board
void InitBoard(int board[SIZE][SIZE]) {
    srand(time(0));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    AddTile(board);
    AddTile(board);
}

// draw the board and tiles
void DrawBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = TILE_PADDING + j * (TILE_SIZE + TILE_PADDING);
            int y = TILE_PADDING + i * (TILE_SIZE + TILE_PADDING);
            DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, LIGHTGRAY);

            if (board[i][j] != 0) {
                DrawText(TextFormat("%d", board[i][j]), x + 35, y + 35, 20, DARKGRAY);
            }
        }
    }
}

// add a new tile to the board
void AddTile(int board[SIZE][SIZE]) {
    int emptyTiles[SIZE * SIZE][2];
    int emptyCount = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                emptyTiles[emptyCount][0] = i;
                emptyTiles[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount > 0) {
        int r = rand() % emptyCount;
        board[emptyTiles[r][0]][emptyTiles[r][1]] = (rand() % 2 + 1) * 2;
    }
}
 
// move and merge tiles - dx is horizontal and dy is vertical (like how it was used in maths)
bool MoveAndMergeTiles(int board[SIZE][SIZE], int dx, int dy) {
    bool moved = false;
    bool merged[SIZE][SIZE] = {false};

    // repeat moves and merges for the entire board
    for (int step = 0; step < SIZE; step++) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                int x = j + dx;
                int y = i + dy;
                
                // make sure new position is within the board and current tile is not empty
                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[i][j] != 0) {
                    // if target is empty move the tile there, clear old position and make moved = true
                    if (board[y][x] == 0) {
                        board[y][x] = board[i][j];
                        board[i][j] = 0;
                        moved = true;
                    } 
                    // if target has same value and not been merged then merge tiles (times by 2)
                    else if (board[y][x] == board[i][j] && !merged[y][x]) {
                        board[y][x] *= 2;
                        board[i][j] = 0;
                        merged[y][x] = true;
                        moved = true;
                    }
                }
            }
        }
    }

    return moved;
}

// check if moves are possible
bool CanMove(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) return true; // if board is empty
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) return true; // merge down
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) return true; // merge right
        }
    }
    return false;
}
