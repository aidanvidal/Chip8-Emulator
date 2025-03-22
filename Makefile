CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/opt/homebrew/include
LDFLAGS = -lglfw -lGLEW -lm -L/opt/homebrew/lib -framework OpenGL
SOURCES = main.c chip8.c graphics.c
EXECUTABLE = chip8_emulator

all: $(EXECUTABLE)

$(EXECUTABLE):
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
