#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "entity.h"
#include "constants.h"
#include "utils.h"

void player_jump(entity_t *self);
void player_update(entity_t *self);
void player_move(entity_t *self);
void player_render(SDL_Renderer *renderer, entity_t player, camera_t camera);
void player_spawn(entity_t player);
void player_throw(entity_t *self);

#endif