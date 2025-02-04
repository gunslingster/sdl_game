#include "platform.h"
#include "constants.h"

platform_t PLATFORMS[100] = {0};

void platform_update_state(platform_t *platform)
{
    // Screen boundaries
    if (platform->rect.x <= 0)
        platform->rect.x = 0;
    if (platform->rect.x >= GRID_WIDTH * TILE_SIZE - platform->rect.w)
        platform->rect.x = GRID_WIDTH * TILE_SIZE - platform->rect.w;
    // Screen boundaries
    if (platform->rect.y <= 0)
        platform->rect.y = 0;
    if (platform->rect.y >= GRID_HEIGHT * TILE_SIZE - platform->rect.w)
        platform->rect.y = GRID_HEIGHT * TILE_SIZE - platform->rect.w;
}

void platform_render(SDL_Renderer *renderer, platform_t platform, int cam_x)
{
    SDL_Rect platform_rect = {platform.rect.x - cam_x, platform.rect.y, platform.rect.w, platform.rect.h};
    SDL_RenderCopy(renderer, platform.platform_texture, NULL, &platform_rect);
}

void platform_render_all(SDL_Renderer *renderer, int cam_x)
{
    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (PLATFORMS[i].rect.x > 0 && PLATFORMS[i].rect.y > 0)
        {
            platform_render(renderer, PLATFORMS[i], cam_x);
        }
    }
}

void platform_init(int w, int h, int x, int y, int vel_x, int vel_y, SDL_Texture *platform_texture)
{
    platform_t platform = {.rect = {.w = w, .h = h, .x = x, .y = y}, .vel_x = vel_x, .vel_y = vel_y, .platform_texture = platform_texture};
    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (PLATFORMS[i].rect.x <= 0 && PLATFORMS[i].rect.y <= 0)
        {
            PLATFORMS[i] = platform;
        }
    }
}