#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "constants.h"
#include "utils.h"

typedef struct player
{
    entity_type_t type;
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
    int max_health;
    int health;
    Movement movement;
    SDL_Texture *texture_left;
    SDL_Texture *texture_right;
    void (*update)(struct player *);
    void (*jump)(struct player *);
    void (*move)(struct player *, const Uint8 *);
    void (*render)(SDL_Renderer *, struct player, camera_t);
} player_t;

void player_jump(player_t *self);
void player_update(player_t *self);
void player_move(player_t *self, const Uint8 *keys);
void player_render(SDL_Renderer *renderer, player_t player, camera_t camera);

#endif