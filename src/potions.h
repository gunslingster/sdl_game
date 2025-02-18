#ifndef POTIONS_H
#define POTIONS_H
#include "item.h"

#define MAX_POTIONS 100

typedef struct potion
{
    /* data */
    COMMON_ITEM_FIELDS
    int health;
    void (*render)(SDL_Renderer *, struct potion, camera_t);
    void (*effect)(struct potion *, entity_t *);
} potion_t;

void potion_render(SDL_Renderer *renderer, potion_t self, camera_t camera);
void potion_render_all(SDL_Renderer *renderer, camera_t camera);
void potion_effect(potion_t *self, entity_t *entity);
void poition_initialize_all();
void potion_spawn();

#endif