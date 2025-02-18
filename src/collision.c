#include "collision.h"
#include "player.h"
#include "icicle.h"
#include "enemies/iceman.h"
#include "platform.h"
#include "projectile.h"

extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];
extern entity_t PLAYER;
extern projectile_t PROJECTILES[100];

static void collision_entity_entity(entity_t *entity1, entity_t *entity2)
{
    int curr_time = SDL_GetTicks();
    if (entity1->state & STATE_ATTACKING)
    {
        return;
    }
    else
    {
        if (!entity1->is_damaged)
        {
            entity1->health -= 5;
            entity1->last_damage = curr_time;
            entity1->is_damaged = 1;
            return;
        }
    }

    if (entity1->state & STATE_BOUNCING)
    {
        return;
    }

    int entity1_top = entity1->rect.y;
    int entity1_bottom = entity1->rect.y + entity1->rect.h;
    int entity1_left = entity1->rect.x;
    int entity1_right = entity1->rect.x + entity1->rect.w;
    int entity2_top = entity2->rect.y;
    int entity2_bottom = entity2->rect.y + entity2->rect.h;
    int entity2_left = entity2->rect.x;
    int entity2_right = entity2->rect.x + entity2->rect.w;

    float bounce_factor = 2;

    entity1->state |= STATE_BOUNCING;
    entity2->state |= STATE_BOUNCING;

    // Collision from the top (entity landing on the platform)
    if (entity1_bottom >= entity2_top && entity1_top < entity2_top &&
        entity1_right > entity2_left && entity1_left < entity2_right)
    {
        entity1->vel_y = -(entity1->vel_y);
    }
    // Collision from the top (entity landing on the platform)
    if (entity2_bottom >= entity1_top && entity2_top < entity1_top &&
        entity2_right > entity1_left && entity2_left < entity1_right)
    {
        entity2->vel_y = -(entity2->vel_y);
    }
    // Collision from below (entity1 hitting their head on the entity2)
    else if (entity1_top <= entity2_bottom && entity1_bottom < entity2_bottom &&
             entity1_right > entity2_left && entity1_left < entity2_right)
    {
        entity1->vel_y = -(entity1->vel_y);
    }
    // Collision from the left (entity1 hitting the right side of the entity2)
    else if (entity1_right >= entity2_left && entity1_left < entity2_left &&
             entity1_bottom > entity2_top && entity1_top < entity2_bottom)
    {
        entity1->vel_x = -(bounce_factor * SPEED);
        entity2->vel_x = 0;
    }
    // Collision from the right (entity1 hitting the left side of the entity2)
    else if (entity1_left <= entity2_right && entity1_right > entity2_right &&
             entity1_bottom > entity2_top && entity1_top < entity2_bottom)
    {
        entity1->vel_x = (bounce_factor * SPEED);
        entity2->vel_x = 0;
    }
}

static void collision_entity_projectile(entity_t *entity, projectile_t *projectile)
{
    if (entity->type == projectile->parent->type)
        return;
    entity->health -= 5;
    projectile->is_active = 0;
    projectile->rect.x = -100;
}

static void collision_entity_platform(entity_t *entity, platform_t *platform)
{
    int entity_top = entity->rect.y;
    int entity_bottom = entity->rect.y + entity->rect.h;
    int entity_left = entity->rect.x;
    int entity_right = entity->rect.x + entity->rect.w;
    int platform_top = platform->rect.y;
    int platform_bottom = platform->rect.y + platform->rect.h;
    int platform_left = platform->rect.x;
    int platform_right = platform->rect.x + platform->rect.w;

    // Collision from the top (entity landing on the platform)
    if (entity_bottom >= platform_top && entity_top < platform_top &&
        entity_right > platform_left && entity_left < platform_right)
    {
        entity->rect.y = platform_top - entity->rect.h;
        entity->vel_y = 0;
        if (entity->state & STATE_JUMPING)
            entity->state ^= STATE_JUMPING;
    }
    // Collision from below (entity hitting their head on the platform)
    else if (entity_top <= platform_bottom && entity_bottom < platform_bottom &&
             entity_right > platform_left && entity_left < platform_right)
    {
        entity->rect.y = platform_bottom;
        entity->vel_y = -(entity->vel_y); // Bounce effect
    }
    // Collision from the left (entity hitting the right side of the platform)
    else if (entity_right >= platform_left && entity_left < platform_left &&
             entity_bottom > platform_top && entity_top < platform_bottom)
    {
        entity->rect.x = platform_left - entity->rect.w;
    }
    // Collision from the right (entity hitting the left side of the platform)
    else if (entity_left <= platform_right && entity_right > platform_right &&
             entity_bottom > platform_top && entity_top < platform_bottom)
    {
        entity->rect.x = platform_right;
    }
}

static void collision_player_icicle(entity_t *player, icicle_t *icicle)
{
    icicle->is_falling = 0;
    player->health -= 2 * icicle->mass;
}

static void collision_icicle_platform(icicle_t *icicle, platform_t *platform)
{
    icicle->is_falling = 0;
}

void collision_check()
{
    for (int i = 0; i < 100; i++)
    {
        if (!ICICLES[i].is_falling)
            continue;
        if (SDL_HasIntersection(&(PLAYER.rect), &(ICICLES[i].rect)))
        {
            collision_player_icicle(&PLAYER, &ICICLES[i]);
        }
        for (int j = 0; j < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); j++)
        {
            if (SDL_HasIntersection(&(ICICLES[i].rect), &(PLATFORMS[j].rect)))
            {
                collision_icicle_platform(&ICICLES[i], &PLATFORMS[j]);
            }
        }
    }

    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (SDL_HasIntersection(&(PLAYER.rect), &(PLATFORMS[i].rect)))
        {
            collision_entity_platform(&PLAYER, &PLATFORMS[i]);
        }
        for (int j = 0; j < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); j++)
        {
            if (SDL_HasIntersection(&(ICEMAN[j].rect), &(PLATFORMS[i].rect)))
            {
                collision_entity_platform(&ICEMAN[j], &PLATFORMS[i]);
            }
        }
    }
    for (int j = 0; j < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); j++)
    {
        if (SDL_HasIntersection(&(ICEMAN[j].rect), &(PLAYER.hitbox)))
        {
            if (PLAYER.state & STATE_ATTACKING && !ICEMAN[j].is_damaged)
            {
                int curr_time = SDL_GetTicks();
                ICEMAN[j].health -= 10;
                ICEMAN[j].last_damage = curr_time;
                ICEMAN[j].is_damaged = 1;
                ICEMAN[j].vel_x = 0;
            }
        }
        if (SDL_HasIntersection(&(ICEMAN[j].rect), &(PLAYER.rect)))
        {
            collision_entity_entity(&PLAYER, &ICEMAN[j]);
        }
        for (int k = 0; k < (sizeof(PROJECTILES) / sizeof(PROJECTILES[0])); k++)
        {
            if (SDL_HasIntersection(&(ICEMAN[j].rect), &(PROJECTILES[k].rect)))
            {
                collision_entity_projectile(&ICEMAN[j], &PROJECTILES[k]);
            }
        }
    }
}