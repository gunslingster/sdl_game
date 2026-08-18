#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "map.h"
#include "platform.h"
#include "texture_manager.h"
#include "player.h"

char cave[GRID_HEIGHT][GRID_WIDTH];
float bg1_x = 0, bg2_x = 0, bg3_x = 0, bg4_x = 0;
const float bg1_speed = 0.25f;
const float bg2_speed = 0.5f;
const float bg3_speed = 0.75f;
const float bg4_speed = 1.0f;
extern entity_t PLAYER;

void background_init()
{
    load_texture(&TEXTURE_MANAGER, "assets/images/background1.png", "background1");
    load_texture(&TEXTURE_MANAGER, "assets/images/background2.png", "background2");
    load_texture(&TEXTURE_MANAGER, "assets/images/background3.png", "background3");
    load_texture(&TEXTURE_MANAGER, "assets/images/background4b.png", "background4");
}

void background_update()
{
}

void background_render(SDL_Renderer *renderer, camera_t camera)
{
    SDL_FRect dest = {0, 0, WIN_WIDTH, WIN_HEIGHT};

    // Render each background twice to allow wrapping
    for (int i = 0; i < 5; i++)
    {
        dest.x = (int)bg1_x - camera.x + i * WIN_WIDTH;
        SDL_RenderTexture(renderer, get_texture(&TEXTURE_MANAGER, "background1"), NULL, &dest);

        dest.x = (int)bg2_x - camera.x + i * WIN_WIDTH;
        SDL_RenderTexture(renderer, get_texture(&TEXTURE_MANAGER, "background2"), NULL, &dest);

        dest.x = (int)bg3_x - camera.x + i * WIN_WIDTH;
        SDL_RenderTexture(renderer, get_texture(&TEXTURE_MANAGER, "background3"), NULL, &dest);

        dest.x = (int)bg4_x - camera.x + i * WIN_WIDTH;
        SDL_RenderTexture(renderer, get_texture(&TEXTURE_MANAGER, "background4"), NULL, &dest);
    }
}

// Initialize cave grid randomly
void cave_init()
{
    srand(time(NULL));
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            cave[y][x] = (rand() % 100 < 45) ? WALL : FLOOR;
        }
    }
}

// Count surrounding walls
static int countWalls(int x, int y)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = x + dx, ny = y + dy;
            if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT)
            {
                count++; // Treat out-of-bounds as wall
            }
            else if (cave[ny][nx] == WALL)
            {
                count++;
            }
        }
    }
    return count;
}

// Smooth cave using cellular automata
void cave_smooth()
{
    char newCave[GRID_HEIGHT][GRID_WIDTH];

    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            int walls = countWalls(x, y);
            newCave[y][x] = (walls >= 5) ? WALL : FLOOR;
        }
    }

    memcpy(cave, newCave, sizeof(cave));
}

void cave_render(SDL_Renderer *renderer, int camX, int camY)
{
    // Convert camera position from pixels to tiles
    int tileStartX = camX / TILE_SIZE;
    int tileStartY = camY / TILE_SIZE;

    for (int y = 0; y <= WIN_HEIGHT / TILE_SIZE; y++)
    {
        for (int x = 0; x <= WIN_WIDTH / TILE_SIZE; x++)
        {
            int worldX = tileStartX + x;
            int worldY = tileStartY + y;

            if (worldX < 0 || worldX >= GRID_WIDTH || worldY < 0 || worldY >= GRID_HEIGHT)
                continue;

            // Corrected tile positioning based on camera offset
            SDL_FRect tile = {
                x * TILE_SIZE - (camX % TILE_SIZE), // Adjust for smooth scrolling
                y * TILE_SIZE - (camY % TILE_SIZE),
                TILE_SIZE, TILE_SIZE};

            SDL_SetRenderDrawColor(renderer, cave[worldY][worldX] == WALL ? 50 : 150, 50, 50, 255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

SDL_Texture *floor_init(SDL_Renderer *renderer)
{
    int floorWidth = GRID_WIDTH * TILE_SIZE;
    int floorHeight = WIN_HEIGHT - GROUND_LEVEL;

    SDL_Surface *surface =
        SDL_CreateSurface(floorWidth, floorHeight, SDL_PIXELFORMAT_RGBA8888);

    if (!surface)
    {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        return NULL;
    }

    if (!SDL_LockSurface(surface))
    {
        SDL_Log("Failed to lock surface: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return NULL;
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;
    int pitch = surface->pitch;

    for (int y = 0; y < surface->h; y += TILE_SIZE)
    {
        for (int x = 0; x < surface->w; x += TILE_SIZE)
        {
            int color = rand() % 40 + 100;

            Uint32 pixelColor =
                SDL_MapSurfaceRGB(surface, color, color / 2, color / 3);

            for (int ty = 0; ty < TILE_SIZE && y + ty < surface->h; ty++)
            {
                for (int tx = 0; tx < TILE_SIZE && x + tx < surface->w; tx++)
                {
                    Uint8 *row =
                        (Uint8 *)surface->pixels + (y + ty) * pitch;

                    ((Uint32 *)row)[x + tx] = pixelColor;
                }
            }
        }
    }

    SDL_UnlockSurface(surface);

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return NULL;
    }

    return texture;
}

void floor_render(SDL_Renderer *renderer, SDL_Texture *floorTexture, int camX)
{
    SDL_FRect srcRect = {camX, 0, WIN_WIDTH + TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL}; // Select part of the texture
    SDL_FRect destRect = {0, GROUND_LEVEL + 50, WIN_WIDTH + TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL};

    SDL_RenderTexture(renderer, floorTexture, &srcRect, &destRect);
}
