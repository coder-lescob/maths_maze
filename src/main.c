#include <stdio.h>
#include "genmaze.h"
#include "renderer.h"

Maze *maze;

void Render(uint32_t *pixels, int width, VideoStatus *status) {
    // all pixels in red
    for (uint32_t y = 0; y < status->vp_height; y++) {
        for (uint32_t x = 0; x < status->vp_width; x++) {
            uint32_t *pix = (uint32_t *)((uint8_t *)pixels + y * width) + x;
            *pix = 0xFFFF0000; // red
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