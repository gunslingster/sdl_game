#ifndef ICICLE_H
#define ICICLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "camera.h"
#include "entity.h"

typedef struct icicle
{
    entity_type_t type;
    SDL_Rect rect;
    SDL_Texture *texture;
    float mass;
    float vel_y;
    int is_falling;
    void (*update)(struct icicle *);
    void (*render)(SDL_Renderer *, struct icicle, camera_t camera);
} icicle_t;

void icicle_initialize(icicle_t *self, int x, int y, int w, int h, int vel_y, int mass, int is_falling, SDL_Texture *);
void icicle_initialize_all(SDL_Texture *init_texture);
void icicle_update(icicle_t *self);
void icicle_update_all();
void icicle_spawn();
void icicle_render(SDL_Renderer *renderer, icicle_t self, camera_t camera);
void icicle_render_all(SDL_Renderer *renderer, camera_t camera);

#endif