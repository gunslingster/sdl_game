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
    TYPE_PROJECTILE
} entity_type_t;

typedef enum
{
    STATE_IDLE = 0,
    STATE_WALKING = 1 << 0,
    STATE_JUMPING = 1 << 1,
    STATE_ATTACKING = 1 << 2,
    STATE_BOUNCING = 1 << 3,
    STATE_THROWING = 1 << 4,
} entity_state_t;

typedef enum
{
    WEAPON_CLUB,
    WEAPON_SPEAR,
    WEAPON_BOW
} entity_weapon_t;

typedef struct entity
{
    entity_type_t type;
    entity_state_t state;
    entity_movement_t movement;
    entity_weapon_t weapon;
    SDL_Rect rect;
    SDL_Rect hitbox;
    int is_active;
    float vel_x;
    float vel_y;
    float jump_str;
    int max_health;
    int health;
    int last_attack;
    int last_throw;
    int bounce_frames;
    float attack_cooldown;
    float throw_cooldown;
    int is_cooldown;
    int is_damaged;
    int last_damage;
    int damage_cooldown;
    int bow_active;
    SDL_Texture *texture;
    void (*update)(struct entity *);
    void (*jump)(struct entity *);
    void (*move)(struct entity *);
    void (*render)(SDL_Renderer *, struct entity, camera_t);
    void (*throw)(struct entity *);
    void (*attack)(struct entity *);

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
void entity_throw(entity_t *self);
void entity_attack(entity_t *self);
void entity_move(entity_t *self);
void entity_render(SDL_Renderer *renderer, entity_t entity, camera_t camera);
void entity_spawn(entity_t entity);

#endif