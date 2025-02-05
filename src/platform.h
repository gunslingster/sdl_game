#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "constants.h"

typedef struct platform
{
    entity_type_t type;
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    SDL_Texture *texture;
    int in_use;
    void (*update)(struct platform *);
    void (*render)(SDL_Renderer *renderer, struct platform *, camera_t camera);
} platform_t;

void platform_update(platform_t *platform);
void platform_render(SDL_Renderer *renderer, platform_t platform, int cam_x);
void platform_spawn(int x, int y, int w, int h, int vel_x, int vel_y, int in_use, SDL_Texture *texture);
void platform_initialize_all(SDL_Texture *init_texture);
void platform_render_all(SDL_Renderer *renderer, camera_t camera);

#endif