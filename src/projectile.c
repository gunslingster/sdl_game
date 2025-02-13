#include "projectile.h"

projectile_t PROJECTILES[100] = {0};

void projectile_spawn(int x, int y, int w, int h, SDL_Texture *texture, int vel_x, int vel_y, int lifetime, int is_active, entity_t *parent)
{
    for (int i = 0; i < (sizeof(PROJECTILES) / sizeof(PROJECTILES[0])); i++)
    {
        if (!PROJECTILES[i].is_active)
        {
            PROJECTILES[i].rect.x = x;
            PROJECTILES[i].rect.y = y;
            PROJECTILES[i].rect.w = w;
            PROJECTILES[i].rect.h = h;
            PROJECTILES[i].texture = texture;
            PROJECTILES[i].type = TYPE_PROJECTILE;
            PROJECTILES[i].vel_x = vel_x;
            PROJECTILES[i].vel_y = vel_y;
            PROJECTILES[i].spawn_time = SDL_GetTicks();
            PROJECTILES[i].lifetime = lifetime;
            PROJECTILES[i].is_active = is_active;
            PROJECTILES[i].parent = parent;
            PROJECTILES[i].update = projectile_update;
            PROJECTILES[i].render = projectile_render;
            return;
        }
    }
}

void projectile_update(projectile_t *self)
{
    int curr_time = SDL_GetTicks();
    if ((curr_time - self->spawn_time) / 1000 >= self->lifetime)
    {
        self->is_active = 0;
        self->rect.x = -100;
        return;
    }
    self->rect.x += self->vel_x;
    self->vel_y += GRAVITY;
    self->rect.y += self->vel_y;
}

void projectile_update_all()
{
    for (int i = 0; i < (sizeof(PROJECTILES) / sizeof(PROJECTILES[0])); i++)
    {
        if (!PROJECTILES[i].is_active)
            continue;
        PROJECTILES[i].update(&PROJECTILES[i]);
    }
}

void projectile_render(SDL_Renderer *renderer, projectile_t self, camera_t camera)
{
    if (!self.is_active)
        return;
    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopy(renderer, self.texture, NULL, &self_rect);
}

void projectile_render_all(SDL_Renderer *renderer, camera_t camera)
{
    for (int i = 0; i < (sizeof(PROJECTILES) / sizeof(PROJECTILES[0])); i++)
    {
        if (!PROJECTILES[i].is_active)
            continue;
        PROJECTILES[i].render(renderer, PROJECTILES[i], camera);
    }
}