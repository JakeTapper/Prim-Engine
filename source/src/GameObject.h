#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
#include <vector>
#include <iostream>

#include "lua\lua.hpp"
#include <LuaBridge\LuaBridge.h>

#include "BoundingBox.h"

class Engine;

struct Vertex {
	glm::vec3 pos; //3D coordinates
	glm::vec3 color; //Color of the vertex
	glm::vec3 normal; //Normal vector
	
	/**Sets the vertex distance from the object center*/
	void scale(GLfloat x, GLfloat y, GLfloat z) {
		pos.x *= x;
		pos.y *= y;
		pos.z *= z;
	}
};

struct Buffers {
	GLuint VAO, VBO;
};

class GameObject {
public:
	std::string name; //Name of the object
	GLfloat lastTime, currentTime, deltaTime; //Time for last loop, current loop, and difference between them
	lua_State* scriptState; //Lua state that holds the object's script
	
	static Engine* engine; //Holds a pointer to the engine (Used for rendering and lua functions)

	/**Adds class information to the lua state*/
	static void addToLua(lua_State* luaState);

	/**Renders the object to the screen*/
	virtual void render() = 0;


	/**Updates the object's physics and runs any scripts*/
	virtual void update();

	//Getters and setters
	float getPositionX() { return position.x; }
	float getPositionY() { return position.y; }
	float getPositionZ() { return position.z; }
	glm::vec3 getPosition() { return position; }

	float getRotationX() { return rotation.x; }
	float getRotationY() { return rotation.y; }
	float getRotationZ() { return rotation.z; }
	glm::vec3 getRotation() { return rotation; }

	float getVelocityX() { return box->velocity.x; }
	float getVelocityY() { return box->velocity.y; }
	float getVelocityZ() { return box->velocity.z; }
	glm::vec3 getVelocity() { return box->velocity; }

	void setPosition(float x, float y, float z) { box->position = glm::vec3(x, y, z); }
	void setRotation(float x, float y, float z) { box->rotation = glm::vec3(x, y, z); }
	void setVelocity(float x, float y, float z) { box->velocity = glm::vec3(x, y, z); }

	BoundingBox* getBoundingBox() { return box; }

protected:
	BoundingBox* box;
	glm::vec3 position; //Objects position
	glm::vec3 rotation;	//Objects rotation state
	std::vector<Vertex> vertices = {}; //List of verices coordinates, color, and normals to be passed to the GPU
	Buffers buffers;

	GameObject(glm::vec3 _position, glm::vec3 _rotation, std::string _name);
};