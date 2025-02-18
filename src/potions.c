#include "potions.h"
#include "texture_manager.h"

potion_t POTIONS[MAX_POTIONS] = {0};

void potion_render(SDL_Renderer *renderer, potion_t self, camera_t camera)
{
    if (!self.is_active)
        return;

    SDL_Rect src_rect = {
        .x = 0,   // X offset in sprite sheet
        .y = 200, // Y position (if all frames are in one row)
        .w = 50,
        .h = 50};

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopy(renderer, self.texture, &src_rect, &self_rect);
}

void potion_render_all(SDL_Renderer *renderer, camera_t camera)
{
    for (int i = 0; i < MAX_POTIONS; i++)
    {
        POTIONS[i].render(renderer, POTIONS[i], camera);
    }
}

void potion_effect(potion_t *self, entity_t *entity)
{
    if (!self->is_active)
        return;
    entity->health += self->health;
    self->is_active = 0;
}

void potion_initialize_all()
{
    SDL_Texture *potion_texture = get_texture(&TEXTURE_MANAGER, "potion");
    if (!potion_texture)
    {
        printf("Failed to load potion texture!\n");
    }

    potion_t init_potion = {.is_active = 0,
                            .health = 10,
                            .rect = {.x = -100, .y = GROUND_LEVEL - 30, .w = 30, .h = 30},
                            .render = potion_render,
                            .effect = potion_effect,
                            .texture = potion_texture};
    for (int i = 0; i < MAX_POTIONS; i++)
    {
        POTIONS[i] = init_potion;
    }
}

void potion_spawn()
{
    for (int i = 0; i < MAX_POTIONS; i++)
    {
        if (!POTIONS[i].is_active)
        {
            POTIONS[i].rect.x = rand() % (GRID_WIDTH * TILE_SIZE);
            POTIONS[i].is_active = 1;
            break;
        }
    }
}