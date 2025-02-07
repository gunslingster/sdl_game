#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "player.h"
#include "icicle.h"
#include "map.h"
#include "platform.h"
#include "camera.h"
#include "utils.h"

extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];
extern camera_t CAMERA;
extern player_t PLAYER;

void render_health_bar(SDL_Renderer *renderer, player_t *player, camera_t camera)
{
    int bar_width = 50; // Set a fixed width
    int bar_height = 10;
    int bar_x = player->rect.x - camera.x;
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
            if (SDL_HasRectIntersection(&(player->rect), &(ICICLES[i].rect)))
            {
                ICICLES[i].is_falling = 0;
                player->health -= 2 * ICICLES[i].mass;
            }
        }
    }

    for (int i = 0; i < (sizeof(PLATFORMS) / sizeof(PLATFORMS[0])); i++)
    {
        if (!SDL_HasRectIntersection(&(player->rect), &(PLATFORMS[i].rect)))
            continue;
        // Handle platform collision from side
        if (player->rect.x < PLATFORMS[i].rect.x && (player->rect.x + player->rect.w) >= PLATFORMS[i].rect.x)
        {
            player->rect.x = PLATFORMS[i].rect.x - player->rect.w;
        }
        if (player->rect.x < PLATFORMS[i].rect.x + PLATFORMS[i].rect.w && (player->rect.x + player->rect.w) > PLATFORMS[i].rect.x + PLATFORMS[i].rect.w)
        {
            player->rect.x = PLATFORMS[i].rect.x + PLATFORMS[i].rect.w;
        }
        // Handle player above platform
        if ((player->rect.y + player->rect.h) >= PLATFORMS[i].rect.y && (player->rect.y - player->rect.h) < PLATFORMS[i].rect.y)
        {
            if (player->is_jumping)
            {
                player->is_jumping = 0;
                player->vel_y = 0;
                player->rect.y = PLATFORMS[i].rect.y - player->rect.h;
            }
            player->rect.y = PLATFORMS[i].rect.y - player->rect.h;
        }
    }
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();

    TTF_Font *font = TTF_OpenFont("./assets/text/Oswald-VariableFont.ttf", 24);
    // if (!font)
    // {
    //     printf("TTF_OpenFont error: %s\n", TTF_GetError());
    //     return 2;
    // }

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Game Window", WIN_WIDTH, WIN_HEIGHT, 0);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_image
    // IMG_Init(IMG_INIT_PNG);
    SDL_Texture *icicleTexture = loadTexture("assets/images/icicle.png", renderer);
    icicle_initialize_all(icicleTexture);

    // Initialize all the platforms
    SDL_Texture *platform_texture = loadTexture("assets/images/2dplatform.png", renderer);
    platform_initialize_all(platform_texture);

    // Initialize cave
    cave_init();
    for (int i = 0; i < 5; i++)
        cave_smooth(); // 5 iterations for a smoother look
    SDL_Texture *floor_texture = floor_init(renderer);
    SDL_Rect floor = {0, GROUND_LEVEL, WIN_WIDTH * TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL};
    platform_spawn(0, GROUND_LEVEL, GRID_WIDTH * TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL, 0, 0, 1, floor_texture);

    // Initialize player
    player_t player = {.type = TYPE_PLAYER, .rect = {100, GROUND_LEVEL - player.rect.h, 50, 50}, .vel_x = 10, .vel_y = 0, .jump_str = -10, .is_jumping = 0, .health = 100, .max_health = 100, .movement = RIGHT, .update = player_update, .jump = player_jump, .render = player_render, .move = player_move};
    player.texture_left = loadTexture("assets/images/caveman_left.png", renderer);
    player.texture_right = loadTexture("assets/images/caveman_right.png", renderer);
    PLAYER = player;

    // Add a random platform for now
    platform_spawn(200, 430, 150, 50, 0, 0, 1, platform_texture);

    // Main game loop
    int running = 1;
    SDL_Event event;
    int timer = 0;

    // Get the number of milliseconds since SDL library initialization.
    Uint64 start_time = SDL_GetTicks();

    while (running)
    {
        Uint64 curr_time = SDL_GetTicks();
        // elapsed time in seconds
        Uint64 elapsed_time = (curr_time - start_time) / 1000;

        while (SDL_PollEvent(&event))
        {
            // messin around, changed above to switch -_- Lol
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = 0;
                break;
            case SDL_EVENT_KEY_DOWN:

                if (event.key.key == SDLK_ESCAPE)
                    running = 0;
                if (event.key.key == SDLK_W && !PLAYER.is_jumping)
                {
                    PLAYER.jump(&PLAYER);
                }

                break;
            }
        }

        // Convert time to string, create new char array, snprintf puts elapsed time into time_text variable
        char time_text[32];
        snprintf(time_text, sizeof(time_text), "Time: %u", elapsed_time);

        // Movement controls
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        PLAYER.move(&PLAYER, keys);

        // Update player state
        PLAYER.update(&PLAYER);

        CAMERA.x = PLAYER.rect.x - (CAMERA.w / 2);
        // camera.y = PLAYER.rect.y - (camera.h / 2);

        // Clamp camera within bounds
        if (CAMERA.x < 0)
            CAMERA.x = 0;
        if (CAMERA.y < 0)
            CAMERA.y = 0;
        if (CAMERA.x > GRID_WIDTH * TILE_SIZE - (WIN_WIDTH))
            CAMERA.x = GRID_WIDTH * TILE_SIZE - (WIN_WIDTH);
        if (CAMERA.y > GRID_HEIGHT * TILE_SIZE - (WIN_HEIGHT))
            CAMERA.y = GRID_HEIGHT * TILE_SIZE - (WIN_HEIGHT);

        if (timer >= 2)
        {
            icicle_spawn();
            timer = 0;
        }
        else
        {
            timer++;
        }

        icicle_update_all();
        check_collisions(&PLAYER);

        if (PLAYER.health <= 0)
            running = 0;

        // Render cave and player
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        cave_render(renderer, CAMERA.x, CAMERA.y);
        platform_render_all(renderer, CAMERA);
        icicle_render_all(renderer, CAMERA);
        player_render(renderer, PLAYER, CAMERA);
        render_health_bar(renderer, &PLAYER, CAMERA);
        // Render time text
        renderText(renderer, font, time_text, 20, 20);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyTexture(icicleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    // IMG_Quit();

    return 0;
}
