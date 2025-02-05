#ifndef UTILS_H
#define UITLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);

#endif