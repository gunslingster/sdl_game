#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef enum entity_type
{
    TYPE_NONE,
    TYPE_PLAYER,
    TYPE_ICICLE,
    TYPE_PLATFORM,
    TYPE_ICEMAN,
} entity_type_t;

#define SPEED 3
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define TILE_SIZE 10
#define GRID_WIDTH (WIN_WIDTH / TILE_SIZE) * 5
#define GRID_HEIGHT (WIN_HEIGHT / TILE_SIZE)
#define GRAVITY 0.5
#define GROUND_LEVEL (WIN_HEIGHT - 100) // Adjusted for procedural ground

#endif