```markdown
# CHIP-8 Emulator

## Description
CHIP-8 Emulator is a simple yet effective emulator for the CHIP-8 (a virtual machine for running basic programs and games). This project is designed to emulate the functionality of the original CHIP-8 system, allowing you to load and run CHIP-8 applications. The emulator uses OpenGL for rendering graphics and provides keyboard input support using GLFW.

## Installation and Setup

### Prerequisites
To build and run the CHIP-8 emulator, you need to have the following installed on your system:

- C compiler (e.g., `gcc`)
- CMake (optional, for easier building)
- OpenGL library
- GLEW library
- GLFW library

### Installation Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/chip8-emulator.git
   cd chip8-emulator
   ```

2. **Install dependencies**:
   On Linux:
   ```bash
   sudo apt-get install build-essential libglew-dev libglfw3-dev
   ```
   On macOS:
   ```bash
   brew install glew glfw
   ```

3. **Build the project**:
   Use the provided `Makefile` to compile the project:
   ```bash
   make
   ```

4. **Run the emulator**:
   After successful compilation, you can run the emulator with a CHIP-8 ROM file:
   ```bash
   ./chip8_emulator <path_to_your_chip8_program>
   ```

5. **Clean up (optional)**:
   To remove the compiled binary, run:
   ```bash
   make clean
   ```

## Usage
1. **Loading Programs**: The emulator requires a CHIP-8 ROM file to run. Provide the path to the ROM file when executing the program.
   ```
   ./chip8-emulator example.ch8
   ```

2. **Controls**: Use the following keyboard mapping to interact with the emulator:
   ```
   CHIP-8 Keypad:    Modern Keyboard:
   1 2 3 C           1 2 3 4
   4 5 6 D           Q W E R
   7 8 9 E           A S D F
   A 0 B F           Z X C V
   ```

3. **Exiting the Emulator**: Press the `ESC` key to close the emulator window.

## ROMS
There are some ROMs included but I did not make any of them. Here is a (link to the repo)[https://github.com/kripod/chip8-roms/tree/master] 

## Dependencies
- **OpenGL**: A cross-language, cross-platform application programming interface for rendering 2D and 3D vector graphics.
- **GLFW**: A library for creating windows with OpenGL contexts and managing input.
- **GLEW**: A cross-platform open-source C/C++ extension loading library.

## Additional Information
- The emulator initializes the CHIP-8 system, loads a program into memory, and executes it in a loop until the user closes the window.
- The graphics are rendered in a 640x320 window, scaled from the original 64x32 CHIP-8 display for better visibility.
- If you encounter errors while initializing the graphics, ensure that OpenGL, GLEW, and GLFW are correctly installed.

Feel free to contribute to the project by submitting issues or pull requests!
```
