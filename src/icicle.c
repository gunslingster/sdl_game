#include "icicle.h"
#include "time.h"

icicle_t ICICLES[100];

void icicle_update_state(icicle_t *icicle)
{
    if (!icicle->is_falling)
    {
        goto exit;
    }
    icicle->vel_y += GRAVITY / 2;
    icicle->rect.y += icicle->vel_y;

    // Ground collision
    if (icicle->rect.y >= GROUND_LEVEL)
    {
        // icicle->rect.y = GROUND_LEVEL;
        icicle->vel_y = 0;
        icicle->is_falling = 0;
    }

    // Screen boundaries
    if (icicle->rect.x <= 0)
        icicle->rect.x = 0;
    if (icicle->rect.x >= GRID_WIDTH * TILE_SIZE - icicle->rect.w)
        icicle->rect.x = GRID_WIDTH * TILE_SIZE - icicle->rect.w;
    // Screen boundaries
    if (icicle->rect.y <= 0)
        icicle->rect.y = 0;
    if (icicle->rect.y >= GRID_HEIGHT * TILE_SIZE - icicle->rect.h)
        icicle->rect.y = GRID_HEIGHT * TILE_SIZE - icicle->rect.h;

exit:
    return;
}

void icicle_update_state_all()
{
    for (int i = 0; i < 100; i++)
    {
        icicle_update_state(&(ICICLES[i]));
    }
}

void icicle_spawn()
{
    // Initialize an icicle with random x position, random size, and random mass
    icicle_t icicle = {.rect = {rand() % (GRID_WIDTH * TILE_SIZE), 0, 50, 50}, .vel_y = 0, .mass = 10, .is_falling = 1};
    for (int i = 0; i < 100; i++)
    {
        if (!ICICLES[i].is_falling)
        {

            ICICLES[i] = icicle;
            break;
        }
    }
}

void icicle_render(icicle_t *icicle, SDL_Texture *icicle_texture, SDL_Renderer *renderer, int camera_x)
{
    if (icicle->is_falling)
    {
        SDL_Rect render_rect = {
            icicle->rect.x - camera_x, // Adjust for camera
            icicle->rect.y,
            icicle->rect.w,
            icicle->rect.h};
        SDL_RenderCopy(renderer, icicle_texture, NULL, &render_rect);
    }
}

void icicle_render_all(SDL_Texture *icicle_texture, SDL_Renderer *renderer, int camera_x)
{
    for (int i = 0; i < 100; i++)
    {
        if (ICICLES[i].is_falling)
            icicle_render(&ICICLES[i], icicle_texture, renderer, camera_x);
    }
}