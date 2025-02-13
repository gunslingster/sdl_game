#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entity.h"

typedef struct projectile
{
    SDL_Rect rect;
    SDL_Texture *texture;
    entity_type_t type;
    int vel_x;
    int vel_y;
    int spawn_time;
    int lifetime;
    int is_active;
    entity_t *parent;
    void (*update)(struct projectile *);
    void (*render)(SDL_Renderer *, struct projectile, camera_t);
} projectile_t;

void projectile_spawn(int x, int y, int w, int h, SDL_Texture *texture, int vel_x, int vel_y, int lifetime, int is_active, entity_t *parent);
void projectile_update(projectile_t *self);
void projectile_update_all();
void projectile_render(SDL_Renderer *renderer, projectile_t self, camera_t camera);
void projectile_render_all(SDL_Renderer *renderer, camera_t camera);

#endif