cmake_minimum_required(VERSION 3.14)
project(sdl_game)

set(CMAKE_CXX_STANDARD 17)

# Add SDL3 and SDL_image as submodules
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/SDL")
set(SDL3_IMAGE_DIR "${CMAKE_SOURCE_DIR}/SDL_image")
set(SDL3_TTF_DIR "${CMAKE_SOURCE_DIR}/SDL_ttf")

# Add SDL3
add_subdirectory(${SDL3_DIR} SDL_build)

# Add SDL_image
set(SDL3IMAGE_VENDORED ON)  # Enables built-in dependencies
add_subdirectory(${SDL3_IMAGE_DIR} SDL_image_build)

# Add SDL_ttf
set(SDL3TTF_VENDORED ON)  # Enables built-in dependencies
add_subdirectory(${SDL3_TTF_DIR} SDL_ttf_build)

# Add game source files
set(SOURCES
    src/main.c
    src/player.c
    src/icicle.c
    src/platform.c
    src/camera.c
    src/map.c
    src/text.c
    src/utils.c
)

# Create the executable
add_executable(main ${SOURCES})

# Include SDL headers
target_include_directories(main PRIVATE ${SDL3_DIR}/include ${SDL3_IMAGE_DIR}/include ${SDL3_TTF_DIR}/include)

# Link SDL3 and SDL_image
target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
