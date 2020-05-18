# f3d
A 3d game engine written in C99. This is mainly a learning tool,
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
- [X] Replace all old GL/GLU function calls with custom math functions
- [X] Update code to modern OpenGL
- [X] Implement model loading from files
- [X] Framebuffers/Cube Framebuffers
- [X] Rewrite objects and lighting to be more flexible
- [X] Multiple lights and scene lighting
- [X] Point and directional shadows
- [ ] Better collision detection and physics routines
- [ ] UI routines
- [ ] Bone animations
- [ ] Deferred rendering
- [ ] Overhaul of error handling
- [ ] Configuration files and/or settings
- [ ] Post processing shaders
- [ ] Add in f3d-img for JPEG and PNG image support
- [ ] HDR and tone mapping
- [ ] Lua support
- [ ] Add spotlight
- [ ] Draw batching
- [ ] Add support for Vulkan and multiple backends
