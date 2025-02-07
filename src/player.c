#include "player.h"
#include "constants.h"

player_t PLAYER = {0};

void player_jump(player_t *self)
{
    if (!self->is_jumping)
    {
        self->vel_y = self->jump_str;
        self->is_jumping = 1;
    }
}

void player_move(player_t *self, const Uint8 *keys)
{
    if (keys[SDL_SCANCODE_A])
    {
        self->rect.x -= self->vel_x;
        self->movement = LEFT;
    }
    if (keys[SDL_SCANCODE_D])
    {
        self->rect.x += self->vel_x;
        self->movement = RIGHT;
    }
}

void player_update(player_t *self)
{
    // Apply gravity and movement
    self->vel_y += GRAVITY;
    self->rect.y += self->vel_y;

    // Screen boundaries
    if (self->rect.x <= 0)
        self->rect.x = 0;
    if (self->rect.x >= GRID_WIDTH * TILE_SIZE - self->rect.w)
        self->rect.x = GRID_WIDTH * TILE_SIZE - self->rect.w;
    // Screen boundaries
    if (self->rect.y <= 0)
        self->rect.y = 0;
    if (self->rect.y >= GRID_HEIGHT * TILE_SIZE - self->rect.w)
        self->rect.y = GRID_HEIGHT * TILE_SIZE - self->rect.w;
}

void player_render(SDL_Renderer *renderer, player_t self, camera_t camera)
{
    SDL_Texture *playerTexture;

    if (self.movement == RIGHT)
        playerTexture = self.texture_right;
    else
        playerTexture = self.texture_left;

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderTexture(renderer, playerTexture, NULL, &self_rect);
}