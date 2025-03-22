#include "chip8.h"
#include <time.h>
#include <unistd.h> // For usleep on Unix-based systems

// Define the screen dimensions for the Chip-8 (64x32 pixels)
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize the Chip-8 system
    initialize_chip8();

    // Load the program into memory
    load_program(argv[1]);

    // Initialize OpenGL and create a window
    initGraphics();

    // Target frame duration for 120Hz (in microseconds)
    const int frame_duration = 1000000 / 120;

    // Main emulation loop
    for (;;) {
        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        emulate_cycle();      // Run one CHIP-8 instruction
        handle_key_input();   // Update input state
        if (draw_flag) {      // Only redraw when needed
            update_graphics();
            draw_flag = false;
        }

        // Calculate elapsed time and sleep for the remaining frame duration
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                            (end_time.tv_nsec - start_time.tv_nsec) / 1000;
        if (elapsed_time < frame_duration) {
            usleep(frame_duration - elapsed_time);
        }
    }

    return EXIT_SUCCESS;
}
