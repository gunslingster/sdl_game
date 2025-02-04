#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "player.h"
#include "icicle.h"
#include "map.h"
#include "platform.h"

extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];

typedef struct camera
{
    int x, y;
    int w, h;
} camera_t;

// Load texture function
SDL_Texture *
loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture)
        printf("Failed to load image: %s\n", IMG_GetError());
    return texture;
}

void render_health_bar(SDL_Renderer *renderer, player_t *player, int camera_x)
{
    int bar_width = 50; // Set a fixed width
    int bar_height = 10;
    int bar_x = player->rect.x - camera_x;
    int bar_y = player->rect.y - 15; // Position above entity

    // Calculate health percentage
    float health_ratio = (float)player->health / player->max_health;
    int green_width = (int)(bar_width * health_ratio);

    // Draw red background (empty health)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect health_bg = {bar_x, bar_y, bar_width, bar_height};
    SDL_RenderFillRect(renderer, &health_bg);

    // Draw green foreground (remaining health)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect health_fg = {bar_x, bar_y, green_width, bar_height};
    SDL_RenderFillRect(renderer, &health_fg);
}

void check_collisions(player_t *player)
{
    for (int i = 0; i < 100; i++)
    {
        if (ICICLES[i].is_falling)
        {
            if (SDL_HasIntersection(&(player->rect), &(ICICLES[i].rect)))
            {
                ICICLES[i].is_falling = 0;
                player->health -= 2 * ICICLES[i].mass;
            }
        }
    }

    // for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    // {
    //     if (!SDL_HasIntersection(&(player->rect), &(PLATFORMS[i].rect)))
    //         continue;
    //     // Handle player above platform
    //     if (player->rect.y >= PLATFORMS[i].rect.y + player->rect.h)
    //     {
    //         if (player->is_jumping)
    //         {
    //             player->is_jumping = 0;
    //             player->vel_y = 0;
    //         }
    //         player->rect.y = PLATFORMS[i].rect.y - player->rect.h;
    //     }
    // }
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
    SDL_Texture *icicleTexture = loadTexture("assets/images/icicle.png", renderer);

    // Initialize cave
    cave_init();
    for (int i = 0; i < 5; i++)
        cave_smooth(); // 5 iterations for a smoother look
    SDL_Texture *floor_texture = floor_init(renderer);

    // Initialize player
    player_t player = {.rect = {100, GROUND_LEVEL, 50, 50}, .vel_x = 10, .vel_y = 0, .jump_str = -10, .is_jumping = 0, .health = 100, .max_health = 100, .movement = RIGHT};
    player.texture_left = loadTexture("assets/images/caveman_left.png", renderer);
    player.texture_right = loadTexture("assets/images/caveman_right.png", renderer);

    // Initialize platforms
    SDL_Texture *platform_texture = loadTexture("assets/images/2dplatform.png", renderer);
    platform_init(150, 50, 200, 480, 0, 0, platform_texture);

    // Main game loop
    int running = 1;
    SDL_Event event;
    camera_t camera = {0, 0, WIN_WIDTH, WIN_HEIGHT};
    int timer = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            // if (event.type == SDL_QUIT)
            //     running = 0;
            // if (event.type == SDL_KEYDOWN)
            // {
            //     if (event.key.keysym.sym == SDLK_ESCAPE)
            //         running = 0;
            //     if (event.key.keysym.sym == SDLK_w && !player.is_jumping)
            //     {
            //         player_jump(&player);
            //     }
            // }

            // messin around, changed above to switch -_- Lol
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:

                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (event.key.keysym.sym == SDLK_w && !player.is_jumping)
                {
                    player_jump(&player);
                }

                break;
            }
        }

        // Movement controls
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        player_move(&player, keys);

        // Update player state
        player_update_state(&player);

        camera.x = player.rect.x - (camera.w / 2);
        // camera.y = player.rect.y - (camera.h / 2);
        // printf("Player position: (%d, %d)\n", player.rect.x, player.rect.y);
        // printf("Camera position: (%d, %d)\n", camera.x, camera.y);

        // Clamp camera within bounds
        if (camera.x < 0)
            camera.x = 0;
        if (camera.y < 0)
            camera.y = 0;
        if (camera.x > GRID_WIDTH * TILE_SIZE - (WIN_WIDTH))
            camera.x = GRID_WIDTH * TILE_SIZE - (WIN_WIDTH);
        if (camera.y > GRID_HEIGHT * TILE_SIZE - (WIN_HEIGHT))
            camera.y = GRID_HEIGHT * TILE_SIZE - (WIN_HEIGHT);

        if (timer >= 2)
        {
            icicle_spawn();
            timer = 0;
        }
        else
        {
            timer++;
        }

        icicle_update_state_all();
        check_collisions(&player);

        if (player.health <= 0)
            running = 0;

        // Render cave and player
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        cave_render(renderer, camera.x, camera.y);
        floor_render(renderer, floor_texture, camera.x);
        platform_render_all(renderer, camera.x);
        icicle_render_all(icicleTexture, renderer, camera.x);
        player_render(renderer, player, camera.x);
        render_health_bar(renderer, &player, camera.x);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(icicleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();

    return 0;
}
