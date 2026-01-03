#include "player.h"

void UpdatePlayer(Player *player, const Uint8 *keyboard) {
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
}