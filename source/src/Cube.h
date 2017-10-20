#pragma once
#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GameObject.h"
#include "Engine.h"

/**Provides information about the cube for construction while allowing for default values*/
struct cubeInfo {
	std::string name = "Cube";
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 color = glm::vec3(0, 0, 0);
	glm::vec3 size = glm::vec3(1, 1, 1);
	GLboolean staticBox = false;
	GLfloat gravity = 9.81;
	GLfloat friction = 0.0;
};

class Cube : protected GameObject {
public:
	/**Creates the object and sets position, rotation, color based on parameters.
	Also creates the vertices list*/
	Cube(cubeInfo info);

	~Cube();

	void render();

	void update();

	static void addToLua(lua_State* luaState);

	//LUA FUNCTIONS
	/**Adds new cube to the engine's list of objects*/
	static bool createNew(luabridge::LuaRef table);

	/**Makes the cube move the direction of the camera*/
	void moveWithCamera(float speed, bool lockY);

	/**Moves the cube perpendicular to the direction of the camera*/
	void moveLeftByCamera(float speed);

	/**Sets the color of the cube*/
	void setColor(float x, float y, float z);
};