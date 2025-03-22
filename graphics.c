#include "chip8.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Window dimensions - scale up the CHIP-8 display for better visibility
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

// Pixel size (for scaling up)
#define PIXEL_SIZE 10

// GLFW window
GLFWwindow* window;

// OpenGL textures
GLuint texture;

// External references to CHIP-8 state
extern unsigned char gfx[64 * 32];
extern unsigned char key[16];

// Keyboard mapping for CHIP-8 hex keypad
// CHIP-8 keypad:    Modern keyboard mapping:
// 1 2 3 C           1 2 3 4
// 4 5 6 D           Q W E R
// 7 8 9 E           A S D F
// A 0 B F           Z X C V
const int keymap[16] = {
    GLFW_KEY_X,    // 0
    GLFW_KEY_1,    // 1
    GLFW_KEY_2,    // 2
    GLFW_KEY_3,    // 3
    GLFW_KEY_Q,    // 4
    GLFW_KEY_W,    // 5
    GLFW_KEY_E,    // 6
    GLFW_KEY_A,    // 7
    GLFW_KEY_S,    // 8
    GLFW_KEY_D,    // 9
    GLFW_KEY_Z,    // A
    GLFW_KEY_C,    // B
    GLFW_KEY_4,    // C
    GLFW_KEY_R,    // D
    GLFW_KEY_F,    // E
    GLFW_KEY_V     // F
};

// Error callback
void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

// Key callback
void key_callback(GLFWwindow* window, int glfwKey, int scancode, int action, int mods) {
    // Exit on ESC
    if (glfwKey == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    
    // Map keyboard to CHIP-8 keypad
    for (int i = 0; i < 16; i++) {
        if (glfwKey == keymap[i]) {
            if (action == GLFW_PRESS) {
                key[i] = 1;
            } else if (action == GLFW_RELEASE) {
                key[i] = 0;
            }
            break;
        }
    }
}

// Initialize OpenGL and create a window
int initGraphics() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    // Set error callback
    glfwSetErrorCallback(error_callback);
    
    // Create window
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHIP-8 Emulator", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Set key callback
    glfwSetKeyCallback(window, key_callback);
    
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW initialization error: %s\n", glewGetErrorString(err));
        return -1;
    }
    
    // Set up OpenGL
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Enable textures
    glEnable(GL_TEXTURE_2D);
    
    // Create texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    printf("Graphics initialized successfully\n");
    return 0;
}

// Update the screen with the CHIP-8 graphics buffer
void update_graphics() {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Convert the gfx array to texture data (white pixels for 1, black for 0)
    unsigned char textureData[64 * 32 * 3];  // RGB data
    for (int i = 0; i < 64 * 32; i++) {
        textureData[i * 3] = gfx[i] ? 255 : 0;       // R
        textureData[i * 3 + 1] = gfx[i] ? 255 : 0;   // G
        textureData[i * 3 + 2] = gfx[i] ? 255 : 0;   // B
    }
    
    // Update texture data
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    
    // Draw a quad with the texture
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_WIDTH, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, WINDOW_HEIGHT);
    glEnd();
    
    // Swap buffers
    glfwSwapBuffers(window);
    
    // Process events
    glfwPollEvents();
}

// Handle key input updates (call this from main loop)
void handle_key_input() {
    // Process pending events (this will trigger our key callback)
    glfwPollEvents();
    
    // Check if window should close
    if (glfwWindowShouldClose(window)) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

// Clean up graphics resources
void terminateGraphics() {
    glDeleteTextures(1, &texture);
    glfwTerminate();
}