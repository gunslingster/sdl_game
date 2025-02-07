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

void collision_queue_process();
void collision_check();
void collision_add(collision_t collision);

#endif