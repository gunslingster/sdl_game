#include "icicle.h"
#include "time.h"
#include "utils.h"

icicle_t ICICLES[100] = {0};

void icicle_update(icicle_t *self)
{
    if (!self->is_falling)
    {
        goto exit;
    }
    self->vel_y += GRAVITY / 2;
    self->rect.y += self->vel_y;

    // Ground collision
    if (self->rect.y >= GROUND_LEVEL)
    {
        // self->rect.y = GROUND_LEVEL;
        self->vel_y = 0;
        self->is_falling = 0;
    }

    // Screen boundaries
    if (self->rect.x <= 0)
        self->rect.x = 0;
    if (self->rect.x >= GRID_WIDTH * TILE_SIZE - self->rect.w)
        self->rect.x = GRID_WIDTH * TILE_SIZE - self->rect.w;
    // Screen boundaries
    if (self->rect.y <= 0)
        self->rect.y = 0;
    if (self->rect.y >= GRID_HEIGHT * TILE_SIZE - self->rect.h)
        self->rect.y = GRID_HEIGHT * TILE_SIZE - self->rect.h;

exit:
    return;
}

void icicle_update_state_all()
{
    for (int i = 0; i < 100; i++)
    {
        if (ICICLES[i].is_falling)
            ICICLES[i].update(&ICICLES[i]);
    }
}

void icicle_initialize(icicle_t *self, int x, int y, int w, int h, int vel_y, int mass, int is_falling)
{
    icicle_t icicle = {.rect = {x, y, w, h}, .vel_y = vel_y, .mass = mass, .is_falling = is_falling, .update = icicle_update, .render = icicle_render};
    self->rect.x = x;
    self->rect.y = y;
    self->rect.w = w;
    self->rect.h = h;
    self->vel_y = vel_y;
    self->mass = mass;
    self->is_falling = is_falling;
    self->update = icicle_update;
    self->render = icicle_render;
}

void icicle_spawn()
{
    // Initialize an icicle with random x position, random size, and random mass
    for (int i = 0; i < 100; i++)
    {
        if (!ICICLES[i].is_falling)
        {

            icicle_initialize(&ICICLES[i], (rand() % (GRID_WIDTH * TILE_SIZE)), 0, 50, 50, 0, 10, 1);
            break;
        }
    }
}

void icicle_render(SDL_Renderer *renderer, icicle_t self, camera_t camera)
{
    if (self.is_falling)
    {
        SDL_Rect render_rect = {
            self.rect.x - camera.x, // Adjust for camera
            self.rect.y,
            self.rect.w,
            self.rect.h};
        SDL_RenderCopy(renderer, self.texture, NULL, &render_rect);
    }
}

void icicle_render_all(SDL_Renderer *renderer, camera_t camera)
{
    for (int i = 0; i < 100; i++)
    {
        if (ICICLES[i].is_falling)
            ICICLES[i].render(renderer, ICICLES[i], camera);
    }
}