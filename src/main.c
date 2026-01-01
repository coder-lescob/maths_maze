#include <stdio.h>
#include <time.h>
#include "types.h"
#include "genmaze.h"
#include "renderer.h"

static Maze *maze;

#define CellSize 16
#define IterationsPerFrame 10

void RenderMaze(uint32_t *pixels, int bytesPerRow, VideoStatus *status) {
    // clear the screen
    for (uint y = 0; y < status->vp_height; y++) {
        for (uint x = 0; x < status->vp_width; x++) {
            uint32_t *pix = GetPix(pixels, bytesPerRow, x, y);
            if (!pix) continue;
            *pix = 0xFF000000; /* black */
        }
    }

    // rendering maze
    if (!maze || !maze->cells) return;

    // loop over each cell
    for (uint y = 0; y < maze->height; y++) {
        for (uint x = 0; x < maze->width; x++) {
            // renders it
            RenderCell(maze, CellSize, x, y, pixels, bytesPerRow);
        }
    }
}

void Update(VideoStatus *status) {
    if (!maze->generated)
        if (IterationsPerFrame > 0 || (status->frameCount % (uint32_t)(1 / IterationsPerFrame)) == 0)
            for (uint i = 0; i < IterationsPerFrame; i++)
                Iteration(maze);
}

void HandleEvents(VideoStatus *status) {
    // if ESC press quit
    if (status->event.type == SDL_KEYDOWN && status->event.key.keysym.sym == SDLK_ESCAPE) status->running = 0;
}

void Init(VideoStatus *status) {
    time_t now = time(0);
    maze = GenerateMaze((status->vp_width-1) / CellSize, (status->vp_height-1) / CellSize, now, 1);
    printf("res %dx%d\n", status->vp_width, status->vp_height);
}

int main(void) {
    StartVideo("maths maze", 0, 0, Init, HandleEvents, Update, RenderMaze);
    FreeMaze(maze);
    return 0;
}