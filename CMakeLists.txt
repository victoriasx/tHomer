cmake_minimum_required(VERSION 3.10)
project(tHomer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set optimization flags for build types
set(CMAKE_CXX_FLAG_DEBUG "-g") # Enable debugging 
set(CMAKE_CXX_FLAG_RELEASE "-O3") # Optimization

# Define paths for SDL2_image if not automatically found
set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib/libSDL2_image.dylib") 

# Find the SDL2 library
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add executable
add_executable(tHomer src/main.cpp src/GraphicsManager.cpp)

# Link SDL2
target_link_libraries(tHomer ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} "-framework CoreGraphics" "-framework CoreFoundation")

# Custom command to copy assets to the build directory
add_custom_command(TARGET tHomer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:tHomer>/assets)
    message("SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")