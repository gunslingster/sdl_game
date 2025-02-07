#ifndef UTILS_H
#define UITLS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);

#endif