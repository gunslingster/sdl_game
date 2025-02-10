#include "collision.h"
#include "player.h"
#include "icicle.h"
#include "enemies/iceman.h"
#include "platform.h"

static collision_t COLLISION_QUEUE[MAX_COLLISIONS] = {0};
extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];
extern entity_t PLAYER;

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
        if (entity->state == STATE_JUMPING)
            entity->state = STATE_IDLE;
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

static void collision_process(collision_t collision)
{
    entity_type_t type1 = collision.type1;
    entity_type_t type2 = collision.type2;

    if ((type1 == TYPE_PLAYER || type1 == TYPE_ICEMAN) && type2 == TYPE_PLATFORM)
    {
        entity_t *entity = (entity_t *)collision.obj1;
        platform_t *platform = (platform_t *)collision.obj2;
        collision_entity_platform(entity, platform);
    }

    else if (type1 == TYPE_PLAYER && type2 == TYPE_ICICLE)
    {
        entity_t *player = (entity_t *)collision.obj1;
        icicle_t *icicle = (icicle_t *)collision.obj2;
        collision_player_icicle(player, icicle);
    }

    else if (type1 == TYPE_ICICLE && type2 == TYPE_PLATFORM)
    {
        icicle_t *icicle = (icicle_t *)collision.obj1;
        platform_t *platform = (platform_t *)collision.obj2;
        collision_icicle_platform(icicle, platform);
    }
}

void collision_check()
{
    for (int i = 0; i < 100; i++)
    {
        if (!ICICLES[i].is_falling)
            continue;
        if (SDL_HasIntersection(&(PLAYER.rect), &(ICICLES[i].rect)))
        {
            collision_t collision = {.obj1 = (void *)&PLAYER, .obj2 = (void *)&(ICICLES[i]), .type1 = PLAYER.type, .type2 = ICICLES[i].type};
            collision_process(collision);
        }
        for (int j = 0; j < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); j++)
        {
            if (SDL_HasIntersection(&(ICICLES[i].rect), &(PLATFORMS[j].rect)))
            {
                collision_t collision = {.obj1 = (void *)&(ICICLES[i]), .obj2 = (void *)&(PLATFORMS[j]), .type1 = TYPE_ICICLE, .type2 = TYPE_PLATFORM};
                collision_process(collision);
            }
        }
    }

    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (SDL_HasIntersection(&(PLAYER.rect), &(PLATFORMS[i].rect)))
        {
            collision_t collision = {.obj1 = (void *)&PLAYER, .obj2 = (void *)&(PLATFORMS[i]), .type1 = PLAYER.type, .type2 = PLATFORMS[i].type};
            collision_process(collision);
        }
        for (int j = 0; j < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); j++)
        {
            if (SDL_HasIntersection(&(ICEMAN[j].rect), &(PLATFORMS[i].rect)))
            {
                collision_t collision = {.obj1 = (void *)&ICEMAN[j], .obj2 = (void *)&(PLATFORMS[i]), .type1 = PLAYER.type, .type2 = PLATFORMS[i].type};
                collision_process(collision);
            }
        }
    }
}

void collision_add(collision_t collision)
{
    for (int i = 0; i < MAX_COLLISIONS; i++)
    {
        if (COLLISION_QUEUE[i].obj1 == NULL)
        {
            COLLISION_QUEUE[i] = collision;
            break;
        }
    }
}

void collision_queue_process()
{
    for (int i = 0; i < MAX_COLLISIONS; i++)
    {
        if (COLLISION_QUEUE[i].obj1 == NULL)
            continue;
        collision_process(COLLISION_QUEUE[i]);
        COLLISION_QUEUE[i].obj1 = NULL;
        COLLISION_QUEUE[i].obj2 = NULL;
    }
}
