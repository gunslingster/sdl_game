#include "player.h"
#include "constants.h"
#include "collision.h"
#include "entity.h"
#include "keys.h"
#include "projectile.h"
#include "texture_manager.h"

entity_t PLAYER = {0};

void player_jump(entity_t *self)
{
    if (!self->is_active)
        return;

    if (!self->state & STATE_BOUNCING)
        entity_jump(self);
}

void player_throw(entity_t *self)
{
    if (self->is_cooldown || (self->state & STATE_ATTACKING) || (self->state & STATE_JUMPING))
        return;
    self->state |= STATE_THROWING;
    projectile_spawn(self->rect.x, self->rect.y + self->rect.h / 2, 10, 10, get_texture(&TEXTURE_MANAGER, "rock"), (self->movement == RIGHT) ? SPEED * 4 : -SPEED * 4, 0, 1, 1, self);
    self->last_throw = SDL_GetTicks();
    self->is_cooldown = 1;
}

void player_move(entity_t *self)
{
    if (!self->is_active)
        return;
    if (self->state & STATE_BOUNCING)
        return;

    if (KEYS[SDL_SCANCODE_A])
    {
        self->vel_x = -3 * SPEED;
        self->movement = LEFT;
        if (!(self->state & STATE_JUMPING) && !(self->state & STATE_ATTACKING) && !(self->state & STATE_THROWING))
            self->state |= STATE_WALKING;
    }
    else if (KEYS[SDL_SCANCODE_D])
    {
        self->vel_x = 3 * SPEED;
        self->movement = RIGHT;
        if (!(self->state & STATE_JUMPING) && !(self->state & STATE_ATTACKING) && !(self->state & STATE_THROWING))
            self->state |= STATE_WALKING;
    }
    else
    {
        if (!(self->state & STATE_JUMPING) && !(self->state & STATE_ATTACKING) && !(self->state & STATE_THROWING))
            self->state |= STATE_IDLE;
        self->vel_x = 0;
    }
}

void player_update(entity_t *self)
{
    printf("ATTACK STATE: %d\n", self->state & STATE_ATTACKING);
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

    int curr_time = SDL_GetTicks();
    if (self->state & STATE_ATTACKING)
    {
        if ((curr_time - self->last_attack) / 1000 >= self->attack_cooldown)
        {
            self->state ^= STATE_ATTACKING;
            self->is_cooldown = 0;
        }
    }
    else if (self->state & STATE_BOUNCING)
    {
        if (self->bounce_frames < 3)
        {
            self->bounce_frames++;
        }
        else
        {
            self->bounce_frames = 0;
            self->vel_x = 0;
            self->state ^= STATE_BOUNCING;
        }
    }
    else if (self->state & STATE_THROWING)
    {
        if ((curr_time - self->last_throw) / 1000 >= self->throw_cooldown)
        {
            self->state ^= STATE_THROWING;
            self->is_cooldown = 0;
        }
    }
}

void player_render(SDL_Renderer *renderer, entity_t self, camera_t camera)
{
    if (!self.is_active)
        return;

    SDL_Rect src_rect = {
        .x = 0, // X offset in sprite sheet
        .y = 0, // Y position (if all frames are in one row)
        .w = 50,
        .h = 50};

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

    if (self.state & STATE_ATTACKING)
        src_rect.x = 50;

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopyEx(renderer, self.texture, &src_rect, &self_rect, 0, NULL, flip);
}

void player_spawn(entity_t player)
{
    PLAYER = player;
}