#include <stdio.h>
#include "types.h"
#include "genmaze.h"
#include "renderer.h"

static Maze *maze;

#define CellSize 32

void RenderMaze(uint32_t *pixels, int bytesPerRow, VideoStatus *status) {
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

void HandleEvents(VideoStatus *status) {
    // if ESC press quit
    if (status->event.type == SDL_KEYDOWN && status->event.key.keysym.sym == SDLK_ESCAPE) status->running = 0;
}

void Init(VideoStatus *status) {
    maze = AllocateMaze(status->vp_width / CellSize, status->vp_height / CellSize);
    printf("res %dx%d\n", status->vp_width, status->vp_height);
}

int main(void) {
    /*StartVideo("maths maze", 0, 0, Init, HandleEvents, NULL, RenderMaze);
    FreeMaze(maze);*/
    Stack stack = NewStack(60);
    Push(stack, 128);
    printf("%d\n", Pop(stack));
    FreeStack(stack);
    return 0;
}