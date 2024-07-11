cmake_minimum_required(VERSION 3.10)
project(mbutt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib/libSDL2_image.dylib") 



# Find the SDL2 library
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)



include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add executable
add_executable(mbutt main.cpp GraphicsManager.cpp)

# Link SDL2
target_link_libraries(mbutt ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

message("SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message("SDL2 image include dirs: ${SDL2_IMAGE_INCLUDE_DIRS}")
message("SDL2 libraries: ${SDL2_LIBRARIES}")
message("SDL2 image libraries: ${SDL2_IMAGE_LIBRARIES}")