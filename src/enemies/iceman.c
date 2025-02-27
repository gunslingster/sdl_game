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

    if (self->health <= 0)
    {
        self->is_active = 0;
        self->rect.x = -100;
        return;
    }

    int curr_time = SDL_GetTicks();
    if ((curr_time - self->last_damage) / 1000 >= self->damage_cooldown)
    {
        self->is_damaged = 0;
    }

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

    if (self->state & STATE_BOUNCING)
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
        if (rand() % 200 == 0)
        {
            int pos_diff = self->rect.x - PLAYER.rect.x;
            if (rand() % 2 == 0)
            {
                if (pos_diff > 0)
                {
                    self->vel_x = -SPEED;
                }
                else if (pos_diff < 0)
                {
                    self->vel_x = SPEED;
                }
            }
            else
            {
                self->vel_x = 0;
            }
        }
    }

    if (rand() % 200 == 0)
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
    if (!self.is_active)
        return;
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
        ICEMAN[i].rect.x = -100;
        ICEMAN[i].rect.y = 400;
        ICEMAN[i].rect.w = 50;
        ICEMAN[i].rect.h = 50;
        ICEMAN[i].texture = texture;
        ICEMAN[i].health = 0;
        ICEMAN[i].damage_cooldown = 1;
        ICEMAN[i].is_damaged = 0;
        ICEMAN[i].jump_str = -10;
        ICEMAN[i].max_health = 20;
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
            ICEMAN[i].health = 20;
            ICEMAN[i].is_active = 1;
            int rand_x = rand() % (GRID_WIDTH * TILE_SIZE);
            while ((abs(rand_x - PLAYER.rect.x)) < 150)
            {
                rand_x = rand() % (GRID_WIDTH * TILE_SIZE);
            }
            ICEMAN[i].rect.x = rand_x;
            break;
        }
    }
}