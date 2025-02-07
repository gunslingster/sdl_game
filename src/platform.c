#include "platform.h"
#include "constants.h"

platform_t PLATFORMS[100] = {0};

void platform_update(platform_t *platform)
{
    if (!platform->in_use)
        return;
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
    SDL_RenderTexture(renderer, platform.texture, NULL, &platform_rect);
}

void platform_render_all(SDL_Renderer *renderer, camera_t camera)
{
    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (PLATFORMS[i].in_use)
        {
            platform_render(renderer, PLATFORMS[i], camera.x);
        }
    }
}

void platform_initialize(platform_t *self, int x, int y, int w, int h, int vel_x, int vel_y, int in_use, SDL_Texture *texture)
{
    self->type = TYPE_PLATFORM;
    self->rect.x = x;
    self->rect.y = y;
    self->rect.w = w;
    self->rect.h = h;
    self->vel_y = vel_y;
    self->in_use = in_use;
    self->update = platform_update;
    self->texture = texture;
    self->render = platform_render;
}

void platform_initialize_all(SDL_Texture *init_texture)
{
    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        platform_initialize(&PLATFORMS[i], 0, 0, 150, 50, 0, 0, 0, init_texture);
    }
}

void platform_spawn(int x, int y, int w, int h, int vel_x, int vel_y, int in_use, SDL_Texture *texture)
{
    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (!PLATFORMS[i].in_use)
        {
            PLATFORMS[i].rect.x = x;
            PLATFORMS[i].rect.y = y;
            PLATFORMS[i].rect.w = w;
            PLATFORMS[i].rect.h = h;
            PLATFORMS[i].vel_x = vel_x;
            PLATFORMS[i].vel_y = vel_y;
            PLATFORMS[i].texture = texture;
            PLATFORMS[i].in_use = in_use;

            break;
        }
    }
}
