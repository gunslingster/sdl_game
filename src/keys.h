#ifndef KEYS_H
#define KEYS_H

#include <SDL2/SDL.h>

// Declare the global variable for keyboard state
extern const Uint8 *KEYS;

// Function to update the keyboard state
void updateKeyboardState();

#endif
