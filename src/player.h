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
* @param player   The player to update.
* @param keyboard The pointer toward the SDL keyboard buffer to access key presses.
* @param maze     The maze that the player is in (for collsion detection).
*/
void UpdatePlayer(Player *player, const Uint8 *keyboard, Maze *maze);

/*
* Moves the player out of collison if there were one
* @param player The player to check collsions.
* @param maze   The maze that the player is in to have collisons.
*/
void ResolveCollisons(Player *player, Maze *maze);