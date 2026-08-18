#ifndef UTILS_H
#define UTILS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

typedef enum Movement
{
    RIGHT,
    LEFT,
    JUMP,
    CRAWL,
} Movement;

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);

#endif
