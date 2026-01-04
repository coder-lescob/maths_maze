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

    // resolve collisons of player with walls
    ResolveCollisons(player, maze);
}

void ResolveCollisons(Player *player, Maze *maze) {
    // compute the position of the player in the cells grid
    uint cellX = (uint32_t)player->x / CellSize, cellY  = (uint32_t)player->y / CellSize;

    // if the player is somehow outside of the maze don't check collison
    if (cellX >= maze->width || cellY >= maze->height) return;

    // get the cell from its index
    uint cellIdx = cellX + cellY * maze->width;
    Cell cell = maze->cells[cellIdx];

    // if there is a left wall and that the player clipped into it snaps him back
    if (cell.leftWall && player->x < cellX * CellSize + HalfPlayerSize) {
        player->x = cellX * CellSize + HalfPlayerSize;
    }

    // if there is a right wall and that the player clipped into it snaps him back
    if (cell.rightWall && player->x > (cellX + 1) * CellSize - HalfPlayerSize) {
        player->x = (cellX + 1) * CellSize - HalfPlayerSize;
    }
    
    // if there is a bottom wall and that the player clipped into it snaps him back
    if (cell.bottomWall && player->y < cellY * CellSize + HalfPlayerSize) {
        player->y = cellY * CellSize + HalfPlayerSize;
    }

    // if there is a top wall and that the player clipped into it snaps him back
    if (cell.topWall && player->y > (cellY + 1) * CellSize - HalfPlayerSize) {
        player->y = (cellY + 1) * CellSize - HalfPlayerSize;
    }
}