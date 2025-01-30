#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct player
{
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
} player_t;

void player_jump(player_t *player);
void player_update_state(player_t *player);
void player_move(player_t *player, const Uint8 *keys);

#endif