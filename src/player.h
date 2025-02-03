#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef enum Movement
{
    RIGHT,
    LEFT,
    JUMP,
    CRAWL,
} Movement;

typedef struct player
{
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
    int max_health;
    int health;
    Movement movement;
    SDL_Texture *texture_left;
    SDL_Texture *texture_right;
} player_t;

void player_jump(player_t *player);
void player_update_state(player_t *player);
void player_move(player_t *player, const Uint8 *keys);
void player_render(SDL_Renderer *renderer, player_t player, int cam_x);

#endif