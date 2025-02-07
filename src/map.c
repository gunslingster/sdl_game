#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "map.h"
#include "platform.h"

char cave[GRID_HEIGHT][GRID_WIDTH];
extern PLATFORMS[100];

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
            SDL_Rect tile = {
                x * TILE_SIZE - (camX % TILE_SIZE), // Adjust for smooth scrolling
                y * TILE_SIZE - (camY % TILE_SIZE),
                TILE_SIZE, TILE_SIZE};

            SDL_SetRenderDrawColor(renderer, cave[worldY][worldX] == WALL ? 50 : 150, 50, 50, 255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

static SDL_Surface *SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    return SDL_CreateSurface(width, height,
                             SDL_GetPixelFormatForMasks(depth, Rmask, Gmask, Bmask, Amask));
}

SDL_Texture *floor_init(SDL_Renderer *renderer)
{
    int floorWidth = GRID_WIDTH * TILE_SIZE; // Make it as wide as the entire world
    int floorHeight = WIN_HEIGHT - GROUND_LEVEL;
    SDL_Surface *surface = SDL_CreateRGBSurface(0, floorWidth, floorHeight, 32, 0, 0, 0, 0);
    if (!surface)
    {
        printf("Failed to create surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_LockSurface(surface);
    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (int y = 0; y < surface->h; y += TILE_SIZE)
    {
        for (int x = 0; x < surface->w; x += TILE_SIZE)
        {
            int color = rand() % 40 + 100; // Random shade variation
            Uint32 pixelColor = SDL_MapRGB(&(surface->format), NULL, color, color / 2, color / 3);

            for (int ty = 0; ty < TILE_SIZE; ty++)
            {
                for (int tx = 0; tx < TILE_SIZE; tx++)
                {
                    int px = (y + ty) * surface->w + (x + tx);
                    if (px < surface->w * surface->h)
                        pixels[px] = pixelColor;
                }
            }
        }
    }

    SDL_UnlockSurface(surface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture)
        printf("Failed to create texture: %s\n", SDL_GetError());

    return texture;

    // SDL_Rect floor = {0, GROUND_LEVEL, WIN_WIDTH * TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL};
}

void floor_render(SDL_Renderer *renderer, SDL_Texture *floorTexture, int camX)
{
    SDL_Rect srcRect = {camX, 0, WIN_WIDTH + TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL}; // Select part of the texture
    SDL_Rect destRect = {0, GROUND_LEVEL + 50, WIN_WIDTH + TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL};

    SDL_RenderTexture(renderer, floorTexture, &srcRect, &destRect);
}
