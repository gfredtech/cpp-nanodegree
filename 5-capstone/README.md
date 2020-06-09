# TicTacToe

This is the captone project for the course [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213), option A.

## Dependencies for Running Locally

- cmake >= 2.8
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- gcc/g++ >= 5.4
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)
- SDL2.0, SDL_TTF2.0 and SDL_IMAGE 2.0
  - Linux(Ubuntu): Install using `sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`
  - Mac and Windows: [Click here for installation instructions](https://wiki.libsdl.org/Installation)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir -p build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./tictactoe`

## Project Structure

```
project
│   README.md
│
└─── assets #font/image assets
│   │
│   └─── font # custom font for game
|   │
│   └─── images # game sprites
│
│
└───include # directory for header files
|    │   field.h      # Field class manages state for Player, Cell and the Gameplay
|    │   sprite.h     # Sprite class for handling image assets
|    │   structs.h    # Helper structs used in Field and Sprite classes
|    │   window.h     # Class for drawing board and images on screen using SDL 2.0
│
└─── src # implementation details for classes(detailed explanation below)
│
└───.clang-format
│
└───.gitignore
│
└───CMakeLists.txt
│
└───README.md
```

### Implementation Details

When the game initially launches, `window.drawFame()` is called inside `src/main.cpp`.
`window.cpp` defines a class that serves as the entrypoint for our game. It handles window-related details(creating window, loading fonts and sprites, handling mouse/keyboard events). All game state is delegated to the `Field` class defined in `field.cpp`. This tracks board state for players, their next moves and whether a player has won or not `Sprite` class is used to handle game assets.
`

### Criteria Addressed:

- The project demonstrates an understanding of C++ functions and control structures. => main.cpp, window.cpp(Window::gameOver)
- The project accepts user input and processes the input. => window.cpp(Window::processEvents)
- The project uses Object Oriented Programming techniques. => Field, Window and Sprite classes
- Classes abstract implementation details from their interfaces. => header files(field.h, sprite.h, window.h) are separated from implementation details
- The project uses destructors appropriately. => Window class
