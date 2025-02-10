#ifndef ICEMAN_H
#define ICEMAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../camera.h"
#include "../constants.h"
#include "../player.h"
#include "../entity.h"

#define MAX_ICEMAN 20

extern entity_t ICEMAN[MAX_ICEMAN];

void iceman_jump(entity_t *self);
// Updates are tied to the player
void iceman_update(entity_t *self);
void iceman_update_all();
void iceman_render(SDL_Renderer *renderer, entity_t iceman, camera_t camera);
void iceman_render_all(SDL_Renderer *renderer, camera_t camera);
void iceman_initialize_all(SDL_Texture *texture);
void iceman_spawn();

#endif