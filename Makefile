#define compiler
CC = gcc 
CFLAGS = -Wall -Wextra #set flags
INCLUDES= -I/usr/include/SDL2/ #include sdl2 header files
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf #link sdl2 libraries
SRCS = $(wildcard src/*.c)
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

