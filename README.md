# f3d
A simple 3d game engine written in C. This is mainly a learning tool,
 but I hope to eventually get it to the point of being a working game!
### Requirements
- CMake
- SDL2 (`SDL2-devel` or `libSDL2-dev`)
- GLEW (`glew-devel` or `libglew-dev`)

### Building
- Make sure you have all the required libraries installed
- Create a new folder for the binaries and object files
- Change directory into build folder, and run `cmake ../src` to build the required Makefiles.
- Run `make` to build and link the executable.

### To do
- [X] Replace all old GLU function calls with custom math functions
- [X] Replace OpenGL 2.x calls with much better OpenGL 3 ones
- [X] Implement model loading from files
- [X] Implement simple lighting and shading
