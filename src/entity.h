#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "camera.h"
#include "constants.h"
#include "utils.h"
#include "entity.h"

typedef enum
{
    MOVEMENT_RIGHT,
    MOVEMENT_LEFT,
} entity_movement_t;

typedef enum
{
    TYPE_NONE,
    TYPE_PLAYER,
    TYPE_ICICLE,
    TYPE_PLATFORM,
    TYPE_ICEMAN,
} entity_type_t;

typedef enum
{
    STATE_IDLE,
    STATE_WALKING,
    STATE_JUMPING,
    STATE_ATTACKING
} entity_state_t;

typedef struct entity
{
    entity_type_t type;
    entity_state_t state;
    entity_movement_t movement;
    SDL_Rect rect;
    int is_active;
    float vel_x;
    float vel_y;
    float jump_str;
    int max_health;
    int health;
    SDL_Texture *texture;
    void (*update)(struct entity *);
    void (*jump)(struct entity *);
    void (*move)(struct entity *);
    void (*render)(SDL_Renderer *, struct entity, camera_t);

    // This union will hold entity specific data
    union
    {
        struct
        {
        } player;

        struct
        {
            Uint64 movement_state_change;
        } enemy;
    };
} entity_t;

void entity_jump(entity_t *self);
void entity_update(entity_t *self);
void entity_move(entity_t *self);
void entity_render(SDL_Renderer *renderer, entity_t entity, camera_t camera);
void entity_spawn(entity_t entity);

#endif