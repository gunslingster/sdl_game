#include "keys.h"
#include <SDL3/SDL.h>

// Define the global variable for keyboard state
const bool *KEYS = NULL;

void updateKeyboardState()
{
    // Update the global keyboard state
    KEYS = SDL_GetKeyboardState(NULL);
}
