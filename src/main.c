#include <stdio.h>
#include "types.h"
#include "genmaze.h"
#include "renderer.h"

Maze *maze;

#define CellSize 32

uint32_t *GetPix(uint32_t *pix, int bytesPerRow, uint32_t x, uint32_t y) {
    if (bytesPerRow <= 0) return NULL;
    return (uint32_t *)((uint8_t *)pix + y * bytesPerRow) + x;
}

void DrawPix(uint32_t *pixels, int bytesPerRow, uint x, uint y, uint32_t col) {
    uint32_t *pix = GetPix(pixels, bytesPerRow, x, y);
    if (!pix) return;
    *pix = col;
}

void DrawVert(uint32_t *pixels, int bytesPerRow, uint x, uint y1, uint y2) {
    for (uint y = y1; y < y2; y++) {
        DrawPix(pixels, bytesPerRow, x, y, 0xFFFFFFFF /* white */ );
    }
}

void DrawHorz(uint32_t *pixels, int bytesPerRow, uint y, uint x1, uint x2) {
    for (uint x = x1; x < x2; x++) {
        DrawPix(pixels, bytesPerRow, x, y, 0xFFFFFFFF /* white */ );
    }
}

void Render(uint32_t *pixels, int bytesPerRow, VideoStatus *status) {
    // rendering maze
    if (!maze || !maze->cells) return;

    // loop over each cell
    for (uint y = 0; y < maze->height; y++) {
        for (uint x = 0; x < maze->width; x++) {
            uint cellIdx = x + y * maze->width;
            // render the cell based on its four walls
            uint pixX = x * CellSize, pixY = y * CellSize;
            uint ax = pixX              , ay = pixY              ;
            uint bx = pixX + CellSize -1, by = pixY              ;
            uint cx = pixX + CellSize -1, cy = pixY + CellSize -1;
            uint dx = pixX              , dy = pixY + CellSize -1;

            //if (status->frameCount == 0) printf("%d; %d\n", x, y);

            /* 
            * walls are:
            * top cd
            * bottom ab
            * right bc
            * left ad
            */
            if (maze->cells[cellIdx].topWall)   DrawHorz(pixels, bytesPerRow, dy, dx, cx);
            if (maze->cells[cellIdx].bottomWall)DrawHorz(pixels, bytesPerRow, ay, ax, bx);
            if (maze->cells[cellIdx].rightWall) DrawVert(pixels, bytesPerRow, bx, by, cy);
            if (maze->cells[cellIdx].leftWall)  DrawVert(pixels, bytesPerRow, ax, ay, dy);
        }
    }
}

void HandleEvents(VideoStatus *status) {
    // if ESC press quit
    if (status->event.type == SDL_KEYDOWN && status->event.key.keysym.sym == SDLK_ESCAPE) status->running = 0;
}

void Init(VideoStatus *status) {
    maze = AllocateMaze(status->vp_width / CellSize - 1, status->vp_height / CellSize - 1);
    printf("res %dx%d\n", status->vp_width, status->vp_height);
}

int main(void) {
    StartVideo("maths maze", 0, 0, Init, HandleEvents, NULL, Render);
    FreeMaze(maze);
    return 0;
}