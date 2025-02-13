#include "texture_manager.h"

texture_manager_t TEXTURE_MANAGER = {0};

void init_texture_manager(texture_manager_t *manager, SDL_Renderer *renderer)
{
    manager->renderer = renderer;
    manager->count = 0;
}

SDL_Texture *load_texture(texture_manager_t *manager, const char *file, const char *key)
{
    // Check if texture is already loaded
    for (int i = 0; i < manager->count; i++)
    {
        if (strcmp(manager->textures[i].key, key) == 0)
        {
            return manager->textures[i].texture;
        }
    }

    // Load new texture
    SDL_Texture *texture = IMG_LoadTexture(manager->renderer, file);
    if (!texture)
    {
        printf("Failed to load texture: %s\n", SDL_GetError());
        return NULL;
    }

    // Store it
    if (manager->count < MAX_TEXTURES)
    {
        strncpy(manager->textures[manager->count].key, key, 128);
        manager->textures[manager->count].texture = texture;
        manager->count++;
    }
    else
    {
        printf("Texture Manager is full!\n");
    }

    return texture;
}

SDL_Texture *get_texture(texture_manager_t *manager, const char *key)
{
    for (int i = 0; i < manager->count; i++)
    {
        if (strcmp(manager->textures[i].key, key) == 0)
        {
            return manager->textures[i].texture;
        }
    }
    return NULL;
}
