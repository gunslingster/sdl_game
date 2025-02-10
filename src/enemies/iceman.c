#include <stdlib.h>
#include <time.h>
#include "iceman.h"
#include "player.h"
#include "constants.h"
#include "collision.h"
#include "utils.h"

iceman_t ICEMAN[MAX_ICEMAN];

void iceman_jump(iceman_t *self)
{
    if (!self->is_jumping)
    {
        self->vel_y = self->jump_str;
        self->is_jumping = 1;
    }
}

void iceman_update(iceman_t *self, entity_t *player)
{
    // Apply gravity and movement
    self->vel_y += GRAVITY;
    self->rect.y += self->vel_y;
    self->rect.x += self->vel_x;

    if (self->vel_x > 0)
    {
        self->movement = RIGHT;
    }
    if (self->vel_x < 0)
    {
        self->movement = LEFT;
    }

    // When player is out of view
    // We want to periodically pace back and forth and change directions
    if (abs(self->rect.x - player->rect.x) > WIN_WIDTH)
    {
        Uint64 curr_time = SDL_GetTicks64();
        if ((curr_time - self->movement_state_change) > 10000)
        {
            if (rand() % 3)
            {
                self->vel_x = -self->vel_x;
            }

            if (rand() % 3)
            {
                self->vel_x = 0;
            }

            self->movement_state_change = SDL_GetTicks64();
        }
    }
    // Otherwise follow the player slowly
    else
    {
        self->vel_x = player->vel_x / 2;
    }

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

void iceman_render(SDL_Renderer *renderer, iceman_t self, camera_t camera)
{
    if (self.health <= 0)
        return;

    SDL_Texture *icemanTexture;

    if (self.movement == RIGHT)
        icemanTexture = self.texture_right;
    else
        icemanTexture = self.texture_left;

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopy(renderer, icemanTexture, NULL, &self_rect);
}

// Initialize all to sensible defaults
void iceman_initialize_all(SDL_Texture *iceman_right, SDL_Texture *iceman_left)
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        ICEMAN[i].type = TYPE_ICEMAN;
        ICEMAN[i].rect.x = 0;
        ICEMAN[i].rect.y = 400;
        ICEMAN[i].rect.w = 50;
        ICEMAN[i].rect.h = 50;
        ICEMAN[i].texture_left = iceman_left;
        ICEMAN[i].texture_right = iceman_right;
        ICEMAN[i].health = 0;
        ICEMAN[i].max_health = 30;
        ICEMAN[i].update = iceman_update;
        ICEMAN[i].render = iceman_render;
        ICEMAN[i].jump = iceman_jump;
    }
}

void iceman_spawn()
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        if (ICEMAN[i].health <= 0)
        {
            ICEMAN[i].health = 30;
            ICEMAN[i].rect.x = rand() % (WIN_WIDTH * TILE_SIZE);
        }
    }
}