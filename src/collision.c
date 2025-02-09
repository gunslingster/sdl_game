#include "collision.h"
#include "player.h"
#include "icicle.h"
#include "enemies/iceman.h"
#include "platform.h"

static collision_t COLLISION_QUEUE[MAX_COLLISIONS] = {0};
extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];
// extern iceman_t ICEMAN[MAX_ICEMAN];
extern player_t PLAYER;

static void collision_iceman_platform(iceman_t *iceman, platform_t *platform)
{
    int iceman_top = iceman->rect.y;
    int iceman_bottom = iceman->rect.y + iceman->rect.h;
    int iceman_left = iceman->rect.x;
    int iceman_right = iceman->rect.x + iceman->rect.w;
    int platform_top = platform->rect.y;
    int platform_bottom = platform->rect.y + platform->rect.h;
    int platform_left = platform->rect.x;
    int platform_right = platform->rect.x + platform->rect.w;

    // Collision from the top (iceman landing on the platform)
    if (iceman_bottom >= platform_top && iceman_top < platform_top &&
        iceman_right > platform_left && iceman_left < platform_right)
    {
        iceman->rect.y = platform_top - iceman->rect.h;
        iceman->vel_y = 0;
        iceman->is_jumping = 0;
    }
    // Collision from below (iceman hitting their head on the platform)
    else if (iceman_top <= platform_bottom && iceman_bottom < platform_bottom &&
             iceman_right > platform_left && iceman_left < platform_right)
    {
        iceman->rect.y = platform_bottom;
        iceman->vel_y = -(iceman->vel_y); // Bounce effect
    }
    // Collision from the left (iceman hitting the right side of the platform)
    else if (iceman_right >= platform_left && iceman_left < platform_left &&
             iceman_bottom > platform_top && iceman_top < platform_bottom)
    {
        iceman->rect.x = platform_left - iceman->rect.w;
    }
    // Collision from the right (iceman hitting the left side of the platform)
    else if (iceman_left <= platform_right && iceman_right > platform_right &&
             iceman_bottom > platform_top && iceman_top < platform_bottom)
    {
        iceman->rect.x = platform_right;
    }
}

static void collision_player_platform(player_t *player, platform_t *platform)
{
    int player_top = player->rect.y;
    int player_bottom = player->rect.y + player->rect.h;
    int player_left = player->rect.x;
    int player_right = player->rect.x + player->rect.w;
    int platform_top = platform->rect.y;
    int platform_bottom = platform->rect.y + platform->rect.h;
    int platform_left = platform->rect.x;
    int platform_right = platform->rect.x + platform->rect.w;

    // Collision from the top (player landing on the platform)
    if (player_bottom >= platform_top && player_top < platform_top &&
        player_right > platform_left && player_left < platform_right)
    {
        player->rect.y = platform_top - player->rect.h;
        player->vel_y = 0;
        player->is_jumping = 0;
    }
    // Collision from below (player hitting their head on the platform)
    else if (player_top <= platform_bottom && player_bottom < platform_bottom &&
             player_right > platform_left && player_left < platform_right)
    {
        player->rect.y = platform_bottom;
        player->vel_y = -(player->vel_y); // Bounce effect
    }
    // Collision from the left (player hitting the right side of the platform)
    else if (player_right >= platform_left && player_left < platform_left &&
             player_bottom > platform_top && player_top < platform_bottom)
    {
        player->rect.x = platform_left - player->rect.w;
    }
    // Collision from the right (player hitting the left side of the platform)
    else if (player_left <= platform_right && player_right > platform_right &&
             player_bottom > platform_top && player_top < platform_bottom)
    {
        player->rect.x = platform_right;
    }
}

static void collision_player_icicle(player_t *player, icicle_t *icicle)
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

    if (type1 == TYPE_PLAYER && type2 == TYPE_PLATFORM)
    {
        player_t *player = (player_t *)collision.obj1;
        platform_t *platform = (platform_t *)collision.obj2;
        collision_player_platform(player, platform);
    }

    // else if (type1 == TYPE_ICEMAN && type2 == TYPE_PLATFORM)
    // {
    //     iceman_t *iceman = (iceman_t *)collision.obj1;
    //     platform_t *platform = (platform_t *)collision.obj2;
    //     collision_iceman_platform(iceman, platform);
    // }

    else if (type1 == TYPE_PLAYER && type2 == TYPE_ICICLE)
    {
        player_t *player = (player_t *)collision.obj1;
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
        if (!SDL_HasIntersection(&(PLAYER.rect), &(PLATFORMS[i].rect)))
            continue;
        collision_t collision = {.obj1 = (void *)&PLAYER, .obj2 = (void *)&(PLATFORMS[i]), .type1 = PLAYER.type, .type2 = PLATFORMS[i].type};
        collision_process(collision);
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
