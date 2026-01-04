#pragma once

#include "types.h"
#include "genmaze.h"

/*
* The structure of Player
* @param x The x position of player
* @param y The y position of player
*/
typedef struct Player {
    float x, y;
} Player;

/*
* Moves the player using inputs from user.
* @param player The player to update
*/
void UpdatePlayer(Player *player, const Uint8 *keyboard, Maze *maze);