#pragma once
#include <stdexcept>
#include <vector>
#include <fstream>
#include <list>
#include <iostream>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "lua\lua.hpp"
#include <LuaBridge\LuaBridge.h>

#include "GameObject.h"
#include "Camera.h"
#include "Cube.h"

extern Camera* cam;

class Engine {
public:
	Engine();
	~Engine();

	/**Runs the init and game loop and handles errors. Takes the window title,
	window width, window height, paths to shader files, and the buffer clear color*/
	void run();

	/**Starts GLFW and GL. Constructs the window and sets the nessicary states. Takes the window title,
	window width, window height, paths to shader files, and the buffer clear color*/
	void init(const char* title, int width, int height, const char* vertexFile, const char* fragmentFile, glm::vec3 _clearColor);

	/**Adds an object to the object list*/
	void addObject(GameObject* object);

	GLuint getProgram();

	Camera* getCamera();

	//Lua functions
	/**Allows lua to access the objects list by searching via name*/
	GameObject* getObject(std::string objectName);

	/**Sets up lua state and adds classes and functions*/
	static void setupLuaState(lua_State* _luaState);

	/**Loads a lua file into an object as a script*/
	static void loadLuaScript(const char* scriptName, GameObject* gameobject);

private:
	GLFWwindow* window;
	GLuint VAO, VBO;
	GLuint program;
	Camera* camera;
	lua_State* luaState;

	/**Color to clear the buffer to for each loop*/
	glm::vec3 clearColor; 
	/**All of the game objects to render and update*/
	std::vector<GameObject*> objects; 

	/**Main game loop where calculations and rendering are handled*/
	void gameLoop();

	/**Renders all of the objects in the engine*/
	void renderObjects();

	/**Updates all of the objects and their scripts in the engine*/
	void updateObjects();

	/**Checks for collisions between all of the objects in the engine*/
	void checkForCollisions();

	/**Reads data from a file*/
	static std::vector<char> readFile(const std::string& filename);
};

/**Handles key input*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

/**Handles mouse input*/
void mouseMove(GLFWwindow * window, GLdouble xpos, GLdouble ypos);