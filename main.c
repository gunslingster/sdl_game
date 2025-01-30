#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "player.h"

// Cave variables
#define WALL '#'
#define FLOOR '.'
char cave[GRID_HEIGHT][GRID_WIDTH];

// Load texture function
SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture)
        printf("Failed to load image: %s\n", IMG_GetError());
    return texture;
}

// Initialize cave grid randomly
void initCave()
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
int countWalls(int x, int y)
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
void smoothCave()
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

// Render cave using SDL
void renderCave(SDL_Renderer *renderer)
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            SDL_Rect tile = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, cave[y][x] == WALL ? 50 : 150, 50, 50, 255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

SDL_Texture *generateNoiseFloor(SDL_Renderer *renderer)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, WIN_WIDTH, WIN_HEIGHT - GROUND_LEVEL, 32, 0, 0, 0, 0);
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
            Uint32 pixelColor = SDL_MapRGB(surface->format, color, color / 2, color / 3);

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
    SDL_FreeSurface(surface);

    if (!texture)
        printf("Failed to create texture: %s\n", SDL_GetError());

    return texture;
}

void renderFloor(SDL_Renderer *renderer, SDL_Texture *floorTexture)
{
    SDL_Rect destRect = {0, GROUND_LEVEL + 50, WIN_WIDTH, WIN_HEIGHT - GROUND_LEVEL};
    SDL_RenderCopy(renderer, floorTexture, NULL, &destRect);
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Game Window",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_image
    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *playerTexture = loadTexture("caveman.png", renderer);

    // Initialize cave
    initCave();
    for (int i = 0; i < 5; i++)
        smoothCave(); // 5 iterations for a smoother look
    SDL_Texture *floorTexture = generateNoiseFloor(renderer);

    // Initialize player
    player_t player = {.rect = {100, GROUND_LEVEL, 50, 50}, .vel_x = 5, .vel_y = 0, .jump_str = -8, .is_jumping = 0};

    // Main game loop
    int running = 1;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (event.key.keysym.sym == SDLK_w && !player.is_jumping)
                {
                    player_jump(&player);
                }
            }
        }

        // Movement controls
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        player_move(&player, keys);

        // Update player state
        player_update_state(&player);

        // Render cave and player
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        renderCave(renderer);
        renderFloor(renderer, floorTexture);
        SDL_RenderCopy(renderer, playerTexture, NULL, &player.rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();

    return 0;
}
