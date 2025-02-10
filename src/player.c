#include "player.h"
#include "constants.h"
#include "collision.h"
#include "entity.h"
#include "keys.h"

entity_t PLAYER = {0};

void player_jump(entity_t *self)
{
    if (!self->is_active)
        return;

    entity_jump(self);
}

void player_move(entity_t *self)
{
    if (!self->is_active)
        return;

    if (KEYS[SDL_SCANCODE_A])
    {
        self->vel_x = -3 * SPEED;
        self->movement = LEFT;
        if (!self->state == STATE_JUMPING)
            self->state = STATE_WALKING;
    }
    else if (KEYS[SDL_SCANCODE_D])
    {
        self->vel_x = 3 * SPEED;
        self->movement = RIGHT;
        if (!self->state == STATE_JUMPING)
            self->state = STATE_WALKING;
    }
    else
    {
        if (!self->state == STATE_JUMPING)
            self->state = STATE_IDLE;
        self->vel_x = 0;
    }
}

void player_update(entity_t *self)
{
    self->rect.x += self->vel_x;
    collision_check();

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
    collision_check();
}

void player_render(SDL_Renderer *renderer, entity_t self, camera_t camera)
{
    if (!self.is_active)
        return;

    SDL_RendererFlip flip;

    switch (self.movement)
    {
    case RIGHT:
        flip = SDL_FLIP_NONE;
        break;
    case LEFT:
        flip = SDL_FLIP_HORIZONTAL;
        break;
    }

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopyEx(renderer, self.texture, NULL, &self_rect, 0, NULL, flip);
}

void player_spawn(entity_t player)
{
    PLAYER = player;
}