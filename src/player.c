#include "player.h"
#include "const.h"

void UpdatePlayer(Player *player, const Uint8 *keyboard, Maze *maze) {
    // up and down are reversed because 0,0 is at top left
    if (keyboard[SDL_SCANCODE_W])
        player->y--;

    if (keyboard[SDL_SCANCODE_S])
        player->y++;

    // right and left are nomals
    if (keyboard[SDL_SCANCODE_D])
        player->x++;

    if (keyboard[SDL_SCANCODE_A])
        player->x--;

    uint cellX = (uint32_t)player->x / CellSize, cellY  = (uint32_t)player->y / CellSize;
    uint cellIdx = cellX + cellY * maze->width;

    if (cellX >= maze->width || cellY >= maze->height) return;

    Cell cell = maze->cells[cellIdx];

    if (cell.leftWall && player->x < cellX * CellSize + HalfPlayerSize) {
        player->x = cellX * CellSize + HalfPlayerSize;
    }

    if (cell.rightWall && player->x > (cellX + 1) * CellSize - HalfPlayerSize) {
        player->x = (cellX + 1) * CellSize - HalfPlayerSize;
    }

    if (cell.bottomWall && player->y < cellY * CellSize + HalfPlayerSize) {
        player->y = cellY * CellSize + HalfPlayerSize;
    }

    if (cell.topWall && player->y > (cellY + 1) * CellSize - HalfPlayerSize) {
        player->y = (cellY + 1) * CellSize - HalfPlayerSize;
    }
}