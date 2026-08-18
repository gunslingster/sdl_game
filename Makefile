#define compiler
CC = gcc 
CFLAGS = -g -Wall -Wextra #set flags
INCLUDES= -I/usr/local/include/SDL3/  -I/usr/local/include/SDL3_image -I/usr/local/include/SDL3_ttf#include sdl3 header files
LIBS = -lSDL3 -lSDL3_image -lSDL3_ttf #link sdl3 libraries
SRCS = $(wildcard src/*.c) $(wildcard src/enemies/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = build/main
#TODO: send object files to build folder
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
all: $(TARGET)
run: $(TARGET)
	./$(TARGET)
.PHONY: all run
clean:
	$(RM) $(OBJS) *~ $(TARGET)

