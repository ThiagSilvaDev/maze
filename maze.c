#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TILE_SIZE 10
#define WIDTH 101
#define HEIGHT 61

int maze[HEIGHT][WIDTH];         // 1 = wall, 0 = way
bool visited[HEIGHT][WIDTH];     // for resolution
bool path[HEIGHT][WIDTH];        // final path

int directions[4][2] = {
    {0, -2}, // up
    {2, 0},  // right
    {0, 2},  // down
    {-2, 0}  // left
};

SDL_Window* window;
SDL_Renderer* renderer;

void drawMaze() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (path[y][x]) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
            } else if (visited[y][x]) {
                SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // blue
            } else if (maze[y][x] == 1) {
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // wall
            } else {
                SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // empty path
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10); 
}

// Fisher-Yates algorithm
void shuffle(int dir[4][2]) {
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int tmpX = dir[i][0], tmpY = dir[i][1];
        dir[i][0] = dir[r][0];
        dir[i][1] = dir[r][1];
        dir[r][0] = tmpX;
        dir[r][1] = tmpY;
    }
}

void generateMaze(int x, int y) {
    maze[y][x] = 0;
    drawMaze();

    shuffle(directions);
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        if (nx > 0 && ny > 0 && nx < WIDTH-1 && ny < HEIGHT-1) {
            if (maze[ny][nx] == 1) {
                maze[ny - directions[i][1]/2][nx - directions[i][0]/2] = 0;
                generateMaze(nx, ny);
            }
        }
    }
}

bool solveMaze(int x, int y) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) return false;
    if (maze[y][x] == 1 || visited[y][x]) return false;

    visited[y][x] = true;
    drawMaze();

    if (x == WIDTH - 2 && y == HEIGHT - 2) {
        path[y][x] = true;
        return true;
    }

    if (solveMaze(x + 1, y) || solveMaze(x, y + 1) ||
        solveMaze(x - 1, y) || solveMaze(x, y - 1)) {
        path[y][x] = true;
        return true;
    }

    return false;
}

int main() {
    srand(time(NULL));

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            maze[y][x] = 1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Maze",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    generateMaze(1, 1);

    SDL_Delay(500);

    solveMaze(1, 1);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        drawMaze();
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
