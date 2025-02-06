#include "collision.h"
#include "player.h"
#include "icicle.h"
#include "platform.h"

collision_t COLLISION_QUEUE[MAX_COLLISIONS] = {0};

void collision_process(collision_t collision)
{
    entity_type_t type1 = collision.type1;
    entity_type_t type2 = collision.type2;

    if (type1 == TYPE_PLAYER && type2 == TYPE_PLATFORM)
    {
        player_t *player = (player_t *)collision.obj1;
        platform_t *platform = (platform_t *)collision.obj2;
    }
}