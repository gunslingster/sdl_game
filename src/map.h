#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
#include "camera.h"

#define WALL '#'
#define FLOOR '.'

void cave_init();
void cave_smooth();
void cave_render(SDL_Renderer *renderer, int camX, int camY);
SDL_Texture *floor_init(SDL_Renderer *renderer);
void floor_render(SDL_Renderer *renderer, SDL_Texture *floorTexture, int camX);
void background_init();
void background_update();
void background_render(SDL_Renderer *renderer, camera_t camera);

#endif