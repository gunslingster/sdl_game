#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#define MAX_TEXTURES 100  // Adjust as needed


typedef struct {
    char key[128];        // Name of the texture (file path or alias)
    SDL_Texture *texture; // SDL Texture
} texture_entry_t;

typedef struct {
    SDL_Renderer *renderer;
    texture_entry_t textures[MAX_TEXTURES];
    int count;
} texture_manager_t;

extern texture_manager_t TEXTURE_MANAGER;

void init_texture_manager(texture_manager_t *manager, SDL_Renderer *renderer);
SDL_Texture *load_texture(texture_manager_t *manager, const char *file, const char *key);
SDL_Texture *get_texture(texture_manager_t *manager, const char *key);

#endif