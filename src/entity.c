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
}

void entity_spawn(entity_t entity)
{
}
