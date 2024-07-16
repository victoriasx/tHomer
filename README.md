# tHomer

![tHomer-optimized](https://github.com/user-attachments/assets/8d05abac-d67f-4922-87aa-b3ad8f234570)

The project is a simple SDL application that displays a window with an image of Homer Simpson. Homer's eyes follow the mouse cursor, and when the cursor is inside the window, a donut moves in place of the cursor icon with a spring-like animation effect. The program allows you to drag the window by holding down the left mouse button and pressing the “q” key to exit. You can also resize the window.

## Prerequisites
- macOS
- CMake 3.10 or higher
- SDL2
- SDL2_image

## How to Build

### macOS

1. **Install SDL2 and SDL2_image**:
    ```bash
    brew install sdl2 sdl2_image
    ```
2. **Create a build directory and navigate into it:**
    ```
    mkdir build
    cd build
    ```
3. **Run CMake to configure the project:**
    ```
    cmake ..
    ```
4. **Build the project:**
    ```
    make
    ````

## How to Run
### macOS
```
./tHomer
```



## Notes
- The code uses CoreGraphics framework to get the global mouse position on macOS. Adjustments may be needed for other operating systems.

## Acknowledgements
- The project was inspired by xeyes.
- The development of this project was assisted by ChatGPT with code suggestions and improvements.
