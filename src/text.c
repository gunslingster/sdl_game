#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "text.h"

//function to display a timer to the screen
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Color text_color = {255, 255, 255, 255}; 

    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, text_color);
    if (!text_surface) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(text_surface); // Free surface before returning
        return;
    }
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};

    //render text onto the screen
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    //cleanup
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}