#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SPEED 1
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
const float GRAVITY = 0.5;
const int GROUND_LEVEL = 100; // Y-position of the ground

typedef struct player
{
    SDL_Rect rect;
    float vel_x;
    float vel_y;
    float jump_str;
    int is_jumping;
} player_t;

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture)
        printf("Failed to load image: %s\n", IMG_GetError());
    return texture;
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("SDL2 Game Window",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *playerTexture = loadTexture("caveman.png", renderer);
    SDL_Rect player_rect = {100, GROUND_LEVEL, 50, 50}; // Position and size
    player_t player;
    player.rect = player_rect;
    player.vel_x = 1;
    player.vel_y = 0;
    player.jump_str = -5;
    player.is_jumping = 0;

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
                    running = 0; // Quit on ESC

                if (event.key.keysym.sym == SDLK_SPACE && !player.is_jumping)
                {
                    player.vel_y = player.jump_str;
                    player.is_jumping = 1;
                }
            }
        }

        player.vel_y += GRAVITY;
        player.rect.y += player.vel_y;

        if (player.rect.y >= GROUND_LEVEL)
        {
            player.rect.y = GROUND_LEVEL;
            player.vel_y = 0;
            player.is_jumping = 0;
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        // if (keys[SDL_SCANCODE_W])
        //     player.rect.y -= SPEED; // Move up
        // if (keys[SDL_SCANCODE_S])
        //     player.rect.y += SPEED; // Move down
        if (keys[SDL_SCANCODE_A])
            player.rect.x -= SPEED; // Move left
        if (keys[SDL_SCANCODE_D])
            player.rect.x += SPEED; // Move right

        if (player.rect.x <= 0)
            player.rect.x = 0;
        if (player.rect.x >= WIN_WIDTH)
            player.rect.x = WIN_WIDTH;
        if (player.rect.y <= 0)
            player.rect.y = 0;
        if (player.rect.y >= WIN_HEIGHT)
            player.rect.y = WIN_HEIGHT;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, playerTexture, NULL, &player.rect);

        // Render to screen
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_DestroyTexture(playerTexture);
    IMG_Quit();

    return 0;
}
