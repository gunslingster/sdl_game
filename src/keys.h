#ifndef KEYS_H
#define KEYS_H

#include <SDL3/SDL.h>

// Declare the global variable for keyboard state
extern const bool *KEYS;

// Function to update the keyboard state
void updateKeyboardState();

#endif
