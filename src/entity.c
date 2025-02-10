#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entity.h"

void entity_jump(entity_t *self)
{
    if (!(self->state == STATE_JUMPING))
    {
        self->vel_y = self->jump_str;
        self->state = STATE_JUMPING;
    }
}

void entity_move(entity_t *self)
{
}

void entity_update(entity_t *self)
{
}

void entity_render(SDL_Renderer *renderer, entity_t self, camera_t camera)
{
    if (!self.is_active)
        return;

    SDL_RendererFlip flip;

    switch (self.movement)
    {
    case RIGHT:
        flip = SDL_FLIP_NONE;
        break;
    case LEFT:
        flip = SDL_FLIP_HORIZONTAL;
        break;
    }

    SDL_Rect self_rect = {self.rect.x - camera.x, self.rect.y, self.rect.w, self.rect.h};
    SDL_RenderCopyEx(renderer, self.texture, NULL, &self_rect, 0, NULL, flip);
}

void entity_spawn(entity_t entity)
{
}
