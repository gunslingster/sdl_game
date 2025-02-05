#include "utils.h"

// Load texture function
SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture)
        printf("Failed to load image: %s\n", IMG_GetError());
    return texture;
}