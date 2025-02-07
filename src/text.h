#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);