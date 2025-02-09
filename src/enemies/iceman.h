#ifndef ICEMAN_H
#define ICEMAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../camera.h"
#include "../constants.h"
#include "../player.h"

#define MAX_ICEMAN 20

typedef struct iceman
{
    entity_type_t type;
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
    int max_health;
    int health;
    Uint64 movement_state_change;
    Movement movement;
    SDL_Texture *texture_left;
    SDL_Texture *texture_right;
    void (*update)(struct iceman *, player_t *);
    void (*jump)(struct iceman *);
    void (*render)(SDL_Renderer *, struct iceman, camera_t);
} iceman_t;

void iceman_jump(iceman_t *self);
// Updates are tied to the player
void iceman_update(iceman_t *self, player_t *player);
void iceman_render(SDL_Renderer *renderer, iceman_t iceman, camera_t camera);
void iceman_initialize_all();

#endif