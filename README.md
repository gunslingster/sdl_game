# SDL Game

First game using **SDL2**

## Installation (Linux)
To install SDL2 and required dependencies, run:
```sh
sudo apt install libsdl2-2.0-0
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install make
sudo apt-get install libsdl2-ttf-dev
```

For SDL3 additional libraries are needed. You also need to clone recursively with the --recursive flag.

## Compilation
**gcc** defined as compiler in Makefile. To compile use command:
```sh
make
```

For SDL 3 first cd into the build dir and run
```sh
cmake --build .
```

## Running the Game
To run the compiled game use command:
```sh
make run
```
## Clean
Delete all compiled files, (object & executable) command:
```sh
make clean
```

## File Structure
```sh
./build contains main executable
./src contains all .c .o .h files
./assets contains all images, sounds, media, etc.
```