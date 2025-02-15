#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
#include "collision.h"
#include "keys.h"
#include "enemies/iceman.h"
#include "texture_manager.h"
#include "projectile.h"

extern icicle_t ICICLES[100];
extern platform_t PLATFORMS[100];
extern camera_t CAMERA;
extern entity_t PLAYER;
extern entity_t ICEMAN[MAX_ICEMAN];
extern projectile_t PROJECTILES;

void render_health_bar(SDL_Renderer *renderer, entity_t *entity, camera_t camera)
{
    int bar_width = 50; // Set a fixed width
    int bar_height = 10;
    int bar_x = entity->rect.x - camera.x;
    int bar_y = entity->rect.y - 15; // Position above entity

    // Calculate health percentage
    float health_ratio = (float)entity->health / entity->max_health;
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

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();

    TTF_Font *font = TTF_OpenFont("./assets/text/Oswald-VariableFont.ttf", 24);
    if (!font)
    {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        return 2;
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
    // Load all the textures into the texture manager
    init_texture_manager(&TEXTURE_MANAGER, renderer);
    SDL_Texture *icicleTexture = load_texture(&TEXTURE_MANAGER, "assets/images/icicle.png", "icicle");
    SDL_Texture *platform_texture = load_texture(&TEXTURE_MANAGER, "assets/images/2dplatform.png", "platform");
    SDL_Texture *player_texture = load_texture(&TEXTURE_MANAGER, "assets/images/caveman_sprite_sheet.png", "player");
    SDL_Texture *iceman_texture = load_texture(&TEXTURE_MANAGER, "assets/images/iceman_right.png", "iceman");
    SDL_Texture *rock_texture = load_texture(&TEXTURE_MANAGER, "assets/images/rock.png", "rock");
    icicle_initialize_all(icicleTexture);

    // Initialize all the platforms
    platform_initialize_all(platform_texture);

    // Initialize cave
    cave_init();
    for (int i = 0; i < 5; i++)
        cave_smooth(); // 5 iterations for a smoother look
    SDL_Texture *floor_texture = floor_init(renderer);
    SDL_Rect floor = {0, GROUND_LEVEL, WIN_WIDTH * TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL};
    platform_spawn(0, GROUND_LEVEL, GRID_WIDTH * TILE_SIZE, WIN_HEIGHT - GROUND_LEVEL, 0, 0, 1, floor_texture);

    // Initialize player
    entity_t player = {.type = TYPE_PLAYER, .rect = {100, GROUND_LEVEL - 50 * 2, 50, 50}, .vel_x = 10, .vel_y = 0, .jump_str = -10, .state = STATE_IDLE, .health = 100, .max_health = 100, .movement = RIGHT, .update = player_update, .jump = entity_jump, .render = player_render, .move = player_move, .throw = player_throw, .is_active = 1, .bounce_frames = 3, .attack = entity_attack, .attack_cooldown = 1, .throw_cooldown = 1, .is_cooldown = 0};
    player_spawn(player);
    PLAYER.texture = player_texture;

    // Add a random iceman for now
    iceman_initialize_all(iceman_texture);

    // Add a random platform for now
    platform_spawn(200, 430, 150, 50, 0, 0, 1, platform_texture);

    // Main game loop
    int running = 1;
    SDL_Event event;
    int timer = 0;

    // Get the number of milliseconds since SDL library initialization.
    Uint64 start_time = SDL_GetTicks64();

    while (running)
    {
        Uint64 curr_time = SDL_GetTicks64();
        // elapsed time in seconds
        Uint64 elapsed_time = (curr_time - start_time) / 1000;

        while (SDL_PollEvent(&event))
        {
            // messin around, changed above to switch -_- Lol
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:

                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (event.key.keysym.sym == SDLK_w && !(PLAYER.state & STATE_JUMPING))
                {
                    PLAYER.jump(&PLAYER);
                }
                if (event.key.keysym.sym == SDLK_SPACE)
                    PLAYER.attack(&PLAYER);
                if (event.key.keysym.sym == SDLK_m)
                    PLAYER.throw(&PLAYER);

                break;
            }
        }

        updateKeyboardState();

        // Convert time to string, create new char array, snprintf puts elapsed time into time_text variable
        char time_text[32];
        snprintf(time_text, sizeof(time_text), "Time: %u", elapsed_time);

        // Movement controls
        PLAYER.move(&PLAYER);

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
        if ((rand() % 500) == 0)
            iceman_spawn();

        if (timer >= 10)
        {
            icicle_spawn();
            timer = 0;
        }
        else
        {
            timer++;
        }

        icicle_update_all();
        iceman_update_all();
        projectile_update_all();
        collision_check();

        if (PLAYER.health <= 0)
            running = 0;

        // Render cave and player
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        cave_render(renderer, CAMERA.x, CAMERA.y);
        platform_render_all(renderer, CAMERA);
        icicle_render_all(renderer, CAMERA);
        iceman_render_all(renderer, CAMERA);
        projectile_render_all(renderer, CAMERA);
        PLAYER.render(renderer, PLAYER, CAMERA);
        for (int i = 0; i < MAX_ICEMAN; i++)
            render_health_bar(renderer, &ICEMAN[i], CAMERA);
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
    IMG_Quit();

    return 0;
}
