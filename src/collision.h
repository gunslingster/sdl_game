#ifndef COLLISION_H
#define COLLISION_H

#include "constants.h"

#define MAX_COLLISIONS 100

typedef struct collsion
{
    void *obj1;
    void *obj2;
    entity_type_t type1;
    entity_type_t type2;
} collision_t;

void collision_process(collision_t collision);

#endif