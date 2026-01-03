#include <stdio.h>
#include <time.h>
#include "types.h"
#include "genmaze.h"
#include "renderer.h"
#include "player.h"

static Maze *maze;
static Player *player;

#define CellSize 16ul
#define IterationsPerFrame 10ul
#define PlayerSize 6
#define HalfPlayerSize PlayerSize / 2

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

    uint cellX = player->x / CellSize, cellY  = player->y / CellSize;

    uint cellIdx = cellX + cellY * maze->width;

    // loop over each cell
    for (uint y = 0; y < maze->height; y++) {
        for (uint x = 0; x < maze->width; x++) {
            // renders it
            RenderCell(maze, CellSize, x, y, pixels, bytesPerRow, cellIdx == x + y * maze->width);
        }
    }

    if (player->x >= HalfPlayerSize && player->x + HalfPlayerSize < status->vp_width && player->y >= HalfPlayerSize && player->y + HalfPlayerSize < status->vp_height){
        // render player pix
        for (int i = -HalfPlayerSize; i < HalfPlayerSize; i++)
            DrawVert(pixels, bytesPerRow, player->x + i, player->y - HalfPlayerSize, player->y + HalfPlayerSize, 0xFFFF0000);
    }
}

void Update(VideoStatus *status) {
    if (!maze->generated)
        if (IterationsPerFrame > 0 || (status->frameCount % (uint32_t)(1 / IterationsPerFrame)) == 0)
            for (uint i = 0; i < IterationsPerFrame; i++)
                Iteration(maze);

    UpdatePlayer(player, status->keyboardState);
}

void HandleEvents(VideoStatus *status) {
    // if ESC press quit
    if (status->event.type == SDL_KEYDOWN && status->event.key.keysym.sym == SDLK_ESCAPE) status->running = 0;
}

void Init(VideoStatus *status) {
    printf("Initialize video, resolution: %dx%d pixels\n", status->vp_width, status->vp_height);

    time_t now = time(0);
    maze = GenerateMaze((status->vp_width-1) / CellSize, (status->vp_height-1) / CellSize, now, 1);

    printf("generate maze, size: %dx%d cells\n", maze->width, maze->height);

    player = (Player *)malloc(sizeof(Player));
    *player = (Player){.x = 8, .y = 8};

    printf("create player at (%.1f,%.1f)\n", player->x, player->y);

    printf("Initialization successful.\n");
}

int main(void) {
    StartVideo("maths maze", 0, 0, Init, HandleEvents, Update, RenderMaze);
    FreeMaze(maze);
    free(player);
    return 0;
}