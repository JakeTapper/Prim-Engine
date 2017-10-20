# Prim Engine

Prim Engine (short for primative engine) is a basic game engine written in C/C++ using Modern OpenGL with GLEW and GLFW. At the moment it's missing a few pretty key components like shadows and real lighting, but it works well nonetheless.

![Example GIF](https://github.com/JakeTapper/Prim-Engine/blob/master/PrimEngineExample.gif?raw=true)

### What's here
There's a framework for a full engine, but it's missing quite a bit. What's here so far?
- GLFW window and mouse/WASD inputs
- OpenGL rendering with basic vertex and fragment shaders
- A basic camera and some control functions
- A working OBB physics engine (Missing torque on collision resolution, so objects don't rotate from collisons just yet)
- Lua scripts with a setup, loop, and onCollison function
So, just the very whispers of what one day might be a big, strong, game engine.

### What's not here (yet)
I plan to do as much with this as I possibly can. Some of the big things are:
- Lighting system
  - This is sort of implimented with one point light that is set up at init. I plan to add full lighting at some point soon
- HUD
- Menus
- Possibly a model creation software that allows the user to create very basic 3D objects
  - I'm planning to have this done with through a click and drag system with vertices and a fairly large grid. I don't plan to have any terribly complex models in this
- Other primative 3D polygon object types
- Sound

### What may eventually come?
There are a few things that I want to put in if I somehow finish all I've listed above. These are big things that I haven't made myself any promises on
- Python scripts. It would be cool to be able to choose to do Lua or Python, or even a combination of both. Not sure about the implimentation though, so I don't know for sure yet
- VR support. I know of at least one VR library, but I haven't looked too much at it. If I can get it done, great. If not, shucks
- Controller support would be cool, but I'm not entirely sure how that's implimented, and I haven't really looked into it
- Online gameplay would be amazing, but at the moment I'm not going to aim for stars that high

### What's not going to be here?
- Textures - Weird, but at the moment I want to see how good I can make this look using just basic vertex coloring
- Complex models for much the same reason as textures. I want to make it a simple engine with a lot of depth of use


### Libraries used
The libraries that I used are:
- GLEW for OpenGL bindings
- GLFW for input and window functions
- GLM for vector and matrix math

Credit to https://learnopengl.com/ for teaching the basics behind GLFW and OpenGL. I'm not quite sure how much of it (that's not as basic as it gets) is still there from the tutorial, but I tried to do as little copying as possible. A the moment, I know that at least the shaders and the way that the vertex universals are set are pretty unchanged, as well as a fair bit of the Camera implimentation.
