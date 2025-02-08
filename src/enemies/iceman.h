#ifndef ICEMAN_H
#define ICEMAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "constants.h"

#define MAX_ICEMAN 20

typedef enum Movement
{
    RIGHT,
    LEFT,
    JUMP,
    CRAWL,
} Movement;

typedef struct iceman
{
    entity_type_t type;
    SDL_Rect rect;
    SDL_Rect prev;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
    int max_health;
    int health;
    Movement movement;
    SDL_Texture *texture_left;
    SDL_Texture *texture_right;
    void (*update)(struct iceman *);
    void (*jump)(struct iceman *);
    void (*move)(struct iceman *, const Uint8 *);
    void (*render)(SDL_Renderer *, struct iceman, camera_t);
} iceman_t;

void iceman_jump(iceman_t *self);
void iceman_update(iceman_t *self);
void iceman_move(iceman_t *self, const Uint8 *keys);
void iceman_render(SDL_Renderer *renderer, iceman_t iceman, camera_t camera);

#endif