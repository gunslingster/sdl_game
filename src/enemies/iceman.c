#include <stdlib.h>
#include <time.h>
#include "iceman.h"
#include "../player.h"
#include "../constants.h"
#include "../collision.h"
#include "../utils.h"

entity_t ICEMAN[MAX_ICEMAN];
extern entity_t PLAYER;

void iceman_jump(entity_t *self)
{
    entity_jump(self);
}

void iceman_update(entity_t *self)
{
    if (!self->is_active)
        return;
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
    if (abs(self->rect.x - PLAYER.rect.x) > WIN_WIDTH)
    {
        Uint64 curr_time = SDL_GetTicks64();
        if ((curr_time - self->enemy.movement_state_change) > 3000)
        {
            switch (rand() % 4)
            {
            case 0:
                self->vel_x = 0;
                break;
            case 1:
                self->vel_x = SPEED;
                break;
            case 2:
                self->vel_x = -SPEED;
                break;
            }

            self->enemy.movement_state_change = SDL_GetTicks64();
        }
    }
    // Otherwise follow the player slowly
    else
    {
        int pos_diff = self->rect.x - PLAYER.rect.x;
        if (pos_diff > 0)
        {
            self->vel_x = -SPEED;
        }
        else if (pos_diff < 0)
        {
            self->vel_x = SPEED;
        }
    }

    if (rand() % 20 == 0)
        self->jump(self);

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

void iceman_update_all()
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        ICEMAN[i].update(&ICEMAN[i]);
    }
}

void iceman_render(SDL_Renderer *renderer, entity_t self, camera_t camera)
{
    entity_render(renderer, self, camera);
}

void iceman_render_all(SDL_Renderer *renderer, camera_t camera)
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        ICEMAN[i].render(renderer, ICEMAN[i], camera);
    }
}

// Initialize all to sensible defaults
void iceman_initialize_all(SDL_Texture *texture)
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        ICEMAN[i].type = TYPE_ICEMAN;
        ICEMAN[i].is_active = 0;
        ICEMAN[i].rect.x = 0;
        ICEMAN[i].rect.y = 400;
        ICEMAN[i].rect.w = 50;
        ICEMAN[i].rect.h = 50;
        ICEMAN[i].texture = texture;
        ICEMAN[i].health = 0;
        ICEMAN[i].jump_str = -10;
        ICEMAN[i].max_health = 30;
        ICEMAN[i].update = iceman_update;
        ICEMAN[i].render = iceman_render;
        ICEMAN[i].jump = iceman_jump;
        ICEMAN[i].enemy.movement_state_change = 0;
    }
}

void iceman_spawn()
{
    for (int i = 0; i < (sizeof(ICEMAN) / sizeof(ICEMAN[0])); i++)
    {
        if (ICEMAN[i].health <= 0 && !ICEMAN[i].is_active)
        {
            ICEMAN[i].health = 30;
            ICEMAN[i].is_active = 1;
            ICEMAN[i].rect.x = rand() % (GRID_WIDTH * TILE_SIZE);
            break;
        }
    }
}