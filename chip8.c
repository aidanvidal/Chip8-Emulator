#include "chip8.h"
#include <stdbool.h>

// Global variables for the Chip-8 system
uint8_t memory[4096];
uint8_t V[16];
uint16_t I;
uint16_t PC;
uint8_t delay_timer;
uint8_t sound_timer;
uint16_t stack[16];
uint8_t SP;
bool draw_flag;
unsigned char gfx[64 * 32];
unsigned short opcode;
unsigned char key[16];

// Fontset for the Chip-8 system (5x5 pixels for each character)
unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Function to initialize the Chip-8 system
void initialize_chip8() {
  // Clear memory
  for (int i = 0; i < 4096; i++) {
    memory[i] = 0;
  }

  // Clear registers
  for (int i = 0; i < 16; i++) {
    V[i] = 0;
  }

  // Initialize other variables
  I = 0;
  PC = 0x200; // Program starts at 0x200
  delay_timer = 0;
  sound_timer = 0;
  SP = 0;
  draw_flag = true;

  // Clear graphics buffer
  for (int i = 0; i < 64 * 32; i++) {
    gfx[i] = 0;
  }

  // Clear stack
  for (int i = 0; i < 16; i++) {
    stack[i] = 0;
  }

  // Clear keypad
  for (int i = 0; i < 16; i++) {
    key[i] = 0;
  }

  // Load fontset into memory
  for (int i = 0; i < 80; i++) {
    memory[i] = chip8_fontset[i];
  }
}

// Load program into memory
void load_program(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open program file");
    exit(EXIT_FAILURE);
  }

  size_t bytes_read =
      fread(&memory[0x200], sizeof(uint8_t), 4096 - 0x200, file);
  if (bytes_read == 0 && ferror(file)) {
    perror("Failed to read program file");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  printf("Program loaded: %zu bytes\n", bytes_read); // Debug statement
  fclose(file);
}

// Emulate one cycle of the Chip-8 system
void emulate_cycle() {
  // Fetch opcode
  opcode = (memory[PC] << 8) | memory[PC + 1];

  // Decode and execute opcode (implementation not shown)
  // This would typically involve a switch statement or a lookup table
  // to handle the various opcodes of the Chip-8 system.

  // Decode opcode
  switch (opcode & 0xF000) {
  case 0x0000:
    switch (opcode & 0x000F) {
    case 0x0000: // 0x00E0: Clears the screen
      for (int i = 0; i < 64 * 32; i++) {
        gfx[i] = 0;
      }
      draw_flag = true;
      PC += 2;
      break;

    case 0x000E: // 0x00EE: Returns from subroutine
      SP--;
      PC = stack[SP];
      PC += 2;
      break;
    }
    break;

  case 0x1000: // Jumps to address NNN
    PC = opcode & 0x0FFF;
    break;

  case 0x2000: // Calls subroutine at NNN
    stack[SP] = PC;
    SP++;
    PC = opcode & 0x0FFF;
    break;

  case 0x3000: // Skips the next instruction if VX equals NN
    if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
      PC += 4;
    } else {
      PC += 2;
    }
    break;

  case 0x4000: // Skips the next instruction if VX does not equal NN
    if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
      PC += 4;
    } else {
      PC += 2;
    }
    break;

  case 0x5000: // Skips the next instruction if VX equals VY
    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
      PC += 4;
    } else {
      PC += 2;
    }
    break;

  case 0x6000: // Sets VX to NN
    V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
    PC += 2;
    break;

  case 0x7000: // Adds NN to VX (carry flag is not changed)
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    PC += 2;
    break;

  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000: // Sets VX to the value of VY.
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      break;
    case 0x0001: // Sets VX to VX or VY. (bitwise OR operation).
      V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
      break;
    case 0x0002: // Sets VX to VX and VY. (bitwise AND operation).
      V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
      break;
    case 0x0003: // Sets VX to VX xor VY
      V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
      break;
    case 0x0004: { // Adds VY to VX. VF is set to 1 when there's an overflow,
                   // and to 0 when there is not.
      uint16_t sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
      V[0xF] = (sum > 0xFF) ? 1 : 0;
      V[(opcode & 0x0F00) >> 8] = sum & 0xFF;
      break;
    }
    case 0x0005: // VY is subtracted from VX. VF is set to 0 when there's an
                 // underflow, and 1 when there is not. (i.e. VF set to 1 if VX
                 // >= VY and 0 if not).
      V[0xF] = (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4]) ? 1 : 0;
      V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
      break;
    case 0x0006: // Shifts VX to the right by 1, then stores the least
                 // significant bit of VX prior to the shift into VF.
      V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x01);
      V[(opcode & 0x0F00) >> 8] >>= 1;
      break;
    case 0x0007: // Sets VX to VY minus VX. VF is set to 0 when there's an
                 // underflow, and 1 when there is not. (i.e. VF set to 1 if VY
                 // >= VX).
      V[0xF] = (V[(opcode & 0x00F0) >> 4] >= V[(opcode & 0x0F00) >> 8]) ? 1 : 0;
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
      break;
    case 0x000E: // Shifts VX to the left by 1, then sets VF to 1 if the most
                 // significant bit of VX prior to that shift was set, or to 0
                 // if it was unset.
      V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
      V[(opcode & 0x0F00) >> 8] <<= 1;
      break;
    }
    PC += 2;
    break;

  case 0x9000: // Skips the next instruction if VX does not equal VY.
    if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
      PC += 4;
    } else {
      PC += 2;
    }
    break;

  case 0xA000: // Sets I to the address NNN
    I = opcode & 0x0FFF;
    PC += 2;
    break;

  case 0xB000: // Jumps to the address NNN plus V0.
    PC = V[0] + (opcode & 0x0FFF);
    break;

  case 0xC000: // Sets VX to the result of a bitwise and operation on a random
               // number (Typically: 0 to 255) and NN
    V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
    PC += 2;
    break;

  case 0xD000: { // Draws a sprite at coordinate (VX, VY) that has a width of 8
                 // pixels and a height of N pixels. Each row of 8 pixels is
                 // read as bit-coded starting from memory location I; I value
                 // does not change after the execution of this instruction. As
                 // described above, VF is set to 1 if any screen pixels are
                 // flipped from set to unset when the sprite is drawn, and to 0
                 // if that does not happen.
    uint8_t X = V[(opcode & 0x0F00) >> 8];
    uint8_t Y = V[(opcode & 0x00F0) >> 4];
    uint8_t height = opcode & 0x000F;
    uint8_t pixel;
    V[0xF] = 0;
    for (int yLine = 0; yLine < height; yLine++) {
      pixel = memory[I + yLine];
      for (int xLine = 0; xLine < 8; xLine++) {
        int x = (X + xLine) % 64;
        int y = (Y + yLine) % 32;
        if ((pixel & (0x80 >> xLine)) != 0) {
          if (gfx[x + (y * 64)] == 1) {
            V[0xF] = 1;
          }
          gfx[x + (y * 64)] ^= 1;
        }
      }
    }
    draw_flag = true;
    PC += 2;
  } break;

  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E: // Skips the next instruction if the key stored in VX(only
                 // consider the lowest nibble) is pressed (usually the next
                 // instruction is a jump to skip a code block)
      if (key[V[(opcode & 0x0F00) >> 8] & 0x0F]) {
        PC += 4;
      } else {
        PC += 2;
      }
      break;
    case 0x00A1: // Skips the next instruction if the key stored in VX(only
                 // consider the lowest nibble) is not pressed (usually the next
                 // instruction is a jump to skip a code block)
      if (!key[V[(opcode & 0x0F00) >> 8] & 0x0F]) {
        PC += 4;
      } else {
        PC += 2;
      }
      break;
    }
    break;

  case 0xF000:
    switch (opcode & 0x00FF) {
    case 0x0007: // Sets VX to the value of the delay timer
      V[(opcode & 0x0F00) >> 8] = delay_timer;
      PC += 2;
      break;
    case 0x000A: { // A key press is awaited, and then stored in VX (blocking
                   // operation, all instruction halted until next key event,
                   // delay and sound timers should continue processing)

      bool key_pressed = false;
      for (int i = 0; i < 16; i++) {
        if (key[i]) {
          V[(opcode & 0x0F00) >> 8] = i;
          key_pressed = true;
          break;
        }
      }
      if (!key_pressed) {
        return; // Wait for a key press
      }
      PC += 2;
    } break;
    case 0x0015: // Sets the delay timer to VX.
      delay_timer = V[(opcode & 0x0F00) >> 8];
      PC += 2;
      break;
    case 0x0018: // Sets the sound timer to VX.
      sound_timer = V[(opcode & 0x0F00) >> 8];
      PC += 2;
      break;
    case 0x001E: // Adds VX to I. VF is not affected
      I += V[(opcode & 0x0F00) >> 8];
      PC += 2;
      break;
    case 0x0029: // Sets I to the location of the sprite for the character in
                 // VX(only consider the lowest nibble). Characters 0-F (in
                 // hexadecimal) are represented by a 4x5 font
      I = (V[(opcode & 0x0F00) >> 8] & 0x0F) * 5;
      PC += 2;
      break;
    case 0x0033: // Stores the binary-coded decimal representation of VX, with
                 // the hundreds digit in memory at location in I, the tens
                 // digit at location I+1, and the ones digit at location I+2
      memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
      memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
      memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
      PC += 2;
      break;
    case 0x0055: // Stores from V0 to VX (including VX) in memory, starting at
                 // address I. The offset from I is increased by 1 for each
                 // value written, but I itself is left unmodified
      for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        memory[I + i] = V[i];
      }
      PC += 2;
      break;
    case 0x0065: // Fills from V0 to VX (including VX) with values from memory,
                 // starting at address I. The offset from I is increased by 1
                 // for each value read, but I itself is left unmodified
      for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        V[i] = memory[I + i];
      }
      PC += 2;
      break;
    }
    break;
  }

  // Update timers
  update_timers();
}

// Update the delay and sound timers
void update_timers() {
  if (delay_timer > 0) {
    --delay_timer;
  }
  if (sound_timer > 0) {
    if (sound_timer == 1) {
      printf("BEEP!\n");
    }
    --sound_timer;
  }
}
