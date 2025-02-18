#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entity.h"

#define COMMON_ITEM_FIELDS \
    SDL_Rect rect;         \
    SDL_Texture *texture;  \
    int is_active;

typedef struct item
{
    COMMON_ITEM_FIELDS
} item_t;

#endif