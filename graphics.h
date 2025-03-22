#ifndef GRAPHICS_H
#define GRAPHICS_H

// Initialize OpenGL and create a window
int initGraphics();

// Update the screen with the CHIP-8 graphics buffer
void update_graphics();

// Handle key input updates
void handle_key_input();

// Clean up graphics resources
void terminateGraphics();

#endif /* GRAPHICS_H */