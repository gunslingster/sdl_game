#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct platform
{
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    SDL_Texture *platform_texture;
} platform_t;

void platform_update_state(platform_t *platform);
void platform_render(SDL_Renderer *renderer, platform_t platform, int cam_x);
void platform_init(int w, int h, int x, int y, int vel_x, int vel_y, SDL_Texture *platform_texture);
void platform_render_all(SDL_Renderer *renderer, int cam_x);

#endif