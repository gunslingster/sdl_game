#include "player.h"
#include "constants.h"

void player_jump(player_t *player)
{
    if (!player->is_jumping)
    {
        player->vel_y = player->jump_str;
        player->is_jumping = 1;
    }
}

void player_move(player_t *player, const Uint8 *keys)
{
    if (keys[SDL_SCANCODE_A])
        player->rect.x -= SPEED;
    if (keys[SDL_SCANCODE_D])
        player->rect.x += SPEED;
}

void player_update_state(player_t *player)
{
    // Apply gravity and movement
    player->vel_y += GRAVITY;
    player->rect.y += player->vel_y;

    // Ground collision
    if (player->rect.y >= GROUND_LEVEL)
    {
        player->rect.y = GROUND_LEVEL;
        player->vel_y = 0;
        player->is_jumping = 0;
    }

    // Screen boundaries
    if (player->rect.x <= 0)
        player->rect.x = 0;
    if (player->rect.x >= WIN_WIDTH - player->rect.w)
        player->rect.x = WIN_WIDTH - player->rect.w;
}