#define compiler
CC = gcc 
CFLAGS = -Wall -Wextra #set flags
INCLUDES= -I/usr/include/SDL2/ #include sdl2 header files
LIBS = -lSDL2 -lSDL2_image #link sdl2 libraries
SRCS = main.c icicle.c map.c player.c
OBJS = $(SRCS:.c=.o)
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	$(RM) $(OBJS) *~ $(TARGET)

