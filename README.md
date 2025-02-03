# SDL Game

First game using **SDL2**

## Installation (Linux)
To install SDL2 and required dependencies, run:
```sh
sudo apt install libsdl2-2.0-0
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install make
```

## Compilation
**gcc** defined as compiler in Makefile. To compile use command:
```sh
make
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

##File Structure
./build contains main executable
./src contains all .c .o .h files
./assets contains all images, sounds, media, etc.
