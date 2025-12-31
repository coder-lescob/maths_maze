#include <stdio.h>
#include "types.h"
#include "genmaze.h"
#include "renderer.h"

Maze *maze;

uint GetPixIdx(uint x, uint y, uint bytesPerRow) {
    return x + (y * bytesPerRow) / sizeof(uint32_t);
}

void RenderVertWall(uint p1, uint p2, uint32_t *pixels, uint bytesPerRow) {
    // assumes x are the same
    /*for (uint i = p1; i < p2; i += bytesPerRow / sizeof(uint32_t)) {
        pixels[i] = 0xFFFFFFFF; // white
    }*/
    /* doesn't work (segmentation fault) */
    for (uint i = p1; i < p2; i ++/*= bytesPerRow / sizeof(uint32_t)*/) {
        pixels[i] = 0xFFFFFFFF; // white
    }
}

void Render(uint32_t *pixels, int bytesPerRow, VideoStatus *status) {
    // rendering maze
    if (!maze) return;

    // loop over each cell
    for (uint y = 0; y < maze->height-1; y++) {
        for (uint x = 0; x < maze->width-1; x++) {
            uint cellIdx = x + y * maze->width;
            // render the cell based on its four walls
            uint pixX = x * 32, pixY = y * 32;
            uint a = GetPixIdx(pixX     , pixY     , bytesPerRow);
            uint b = GetPixIdx(pixX + 31, pixY     , bytesPerRow);
            uint c = GetPixIdx(pixX + 31, pixY + 31, bytesPerRow);
            uint d = GetPixIdx(pixX     , pixY + 31, bytesPerRow);

            /* 
            * walls are:
            * top cd
            * bottom ab
            * right bc
            * left ad
            */
            pixels[a] = 0xFFFF0000; // red
            pixels[b] = 0xFF00FF00; // green
            pixels[c] = 0xFF0000FF; // blue
            pixels[d] = 0xFFFFFFFF; // white

            for (uint i = a; i < b; i++)
                pixels[i] = 0xFFFFFF00;

            for (uint i = d; i < c; i++)
                pixels[i] = 0xFFFF00FF;
        }
    }
}

void HandleEvents(VideoStatus *status) {
    if (status->event.type == SDL_KEYDOWN && status->event.key.keysym.sym == SDLK_ESCAPE) status->running = 0;
}

void Init(VideoStatus *status) {
    maze = AllocateMaze(100, 100);
}

int main(void) {
    StartVideo("maths maze", 0, 0, Init, HandleEvents, NULL, Render);
    FreeMaze(maze);
    return 0;
}