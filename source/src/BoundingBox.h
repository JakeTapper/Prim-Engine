#pragma once
#include <cstdio>
#include <math.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\projection.hpp>
#include <glm\gtx\string_cast.hpp>

class BoundingBox {
public:
	glm::vec3 vertices[8];

	glm::vec3 scale;

	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec3 rotation;
	glm::vec3 rotVelocity;

	/**Whether or not the box is static*/
	int staticBox;

	/**The amount of velocity that it removed per second in order to slow the objects movement*/
	GLfloat friction;
	/**The amount of velocity that is added downwards per second to the y axis 
	Should usually be positive, since the number is subtracted from the velocity*/
	GLfloat gravity;

	BoundingBox(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, int _staticBox, GLfloat _friction, GLfloat _gravity);

	/**Checks for a collision with another box and returns the shortest vector for resolving
	the collision*/
	glm::vec3 checkCollisionWith(BoundingBox* other);

	/**Returns the width of the box at the widest part*/
	GLfloat maxWidth();

	/**Updates the box*/
	void update(GLfloat deltaTime);


private:
	void setScale(glm::vec3 _scale);

	glm::vec3 getScale();

	/**Returns the length of the vector, while also keeping it signed*/
	static GLfloat sinLength(glm::vec3 vec);
	static GLfloat sinLength(glm::vec4 vec);
};