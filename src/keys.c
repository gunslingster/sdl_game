#include "keys.h"
#include <SDL2/SDL.h>

// Define the global variable for keyboard state
const Uint8 *KEYS = NULL;

void updateKeyboardState()
{
    // Update the global keyboard state
    KEYS = SDL_GetKeyboardState(NULL);
}
