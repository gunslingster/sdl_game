#ifndef ICICLE_H
#define ICICLE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"

typedef struct icicle
{
    SDL_Rect rect;
    float mass;
    float vel_y;
    int is_falling;
} icicle_t;

void icicle_update_state(icicle_t *icicle);
void icicle_update_state_all();
void icicle_spawn();
void icicle_render(icicle_t *icicle, SDL_Texture *icicle_texture, SDL_Renderer *renderer, int camera_x);
void icicle_render_all(SDL_Texture *icicle_texture, SDL_Renderer *renderer, int camera_x);

#endif