#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);