# Prim Engine

Prim Engine (short for primative engine) is a basic game engine written in C/C++ using Modern OpenGL with GLEW and GLFW. At the moment it's not much more than a framework for a graphics engine, but I plan to add a basic physics engine as well as menus and other such common features

### What's here
There's not much at the moment, just some basic graphics that don't even include lights. What's actually done?
- GLFW window and mouse/WASD inputs
- OpenGL rendering with **very** basic vertex and fragment shaders
- A basic game loop structure
- A Camera object which handles the WASD input and FPS style mouse movement
- A GameObject class, which is a very basic class that defines what an object in the engine must do at the very least
- A linked list of GameObjects with traversal methods to call each objects render and update functions, which are called every loop
- A GameScript class, which is much the same as a GameObject class, but for scripts
- A linked list of GameScripts, which also has a transveral method for the GameScript's run function
So, just the very whispers of what one day might be a big, strong, game engine.

### What's not here (yet)
I plan to do as much with this as I possibly can. Some of the big ones are:
- A physics engine to handle collisions and the like
- Lua and Python scripts to allow for a more flexible game design and easier scripting
- Lighting system
- HUD
- Menus
- Possibly a model creation software that allows the user to create very basic 3D objects
  - I'm planning to have this done with through a click and drag system with vertices and a fairly large grid. I don't plan to have any terribly complex models in this
- Other primative 3D polygon object types

### What may eventually come?
There are a few things that I want to put in if I somehow finish all I've listed above. These are big things that I haven't made myself any promises on
- VR support. I know of at least one VR library, but I haven't looked too much at it. If I can get it done, great. If not, shucks
- Controller support would be cool, but I'm not entirely sure how that's implimented, and I haven't really looked into it
- Online gameplay would be amazing to play on something that I made, but at the moment I'm not going to aim for stars that high

### What's not going to be here?
- Textures - Weird, but at the moment I want to see how good I can make this look using just basic vertex coloring
- Complex models for much the same reason as textures. I want to make it a simple engine with a lot of depth of use


### Libraries used
The libraries that I used are:
- GLEW for OpenGL bindings
- GLFW for input and window functions
- GLM for vector and matrix math

Credit to https://learnopengl.com/ for the basics of the code. I'm not quite sure how much of it (that's not as basic as it gets) is still there from the tutorial, but I tried to do as little copying as possible. A the moment, I know that at least the shaders and the way that those universals are set are pretty unchanged, as well as a fair bit of the Camera implimentation.
