#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Chip-8 system variables (defined in chip8.c)
extern uint8_t memory[4096];
extern uint8_t V[16];
extern uint16_t I;
extern uint16_t PC;
extern uint8_t delay_timer;
extern uint8_t sound_timer;
extern uint16_t stack[16];
extern uint8_t SP;
extern bool draw_flag;
extern unsigned char gfx[64 * 32];
extern unsigned char key[16];

// Initialize the Chip-8 system
void initialize_chip8();

// Load program into memory
void load_program(const char *filename);

// Emulate one cycle of the Chip-8 system
void emulate_cycle();

// Update timers
void update_timers();

// Graphics functions (defined in graphics.c)
int initGraphics();
void update_graphics();
void handle_key_input();
void terminateGraphics();

#endif /* CHIP8_H */