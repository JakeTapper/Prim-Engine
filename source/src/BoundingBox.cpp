#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation, int _staticBox, GLfloat _friction, GLfloat _gravity)
{
	position = _position;
	scale = _scale;
	rotation = _rotation;
	staticBox = _staticBox;
	gravity = _gravity;

	friction = _friction;

	vertices[0] = glm::vec3(-0.5, -0.5, -0.5) * scale;
	vertices[1] = glm::vec3(-0.5, -0.5,  0.5) * scale;
	vertices[2] = glm::vec3(-0.5,  0.5, -0.5) * scale;
	vertices[3] = glm::vec3(-0.5,  0.5,  0.5) * scale;
	vertices[4] = glm::vec3( 0.5, -0.5, -0.5) * scale;
	vertices[5] = glm::vec3( 0.5, -0.5,  0.5) * scale;
	vertices[6] = glm::vec3( 0.5,  0.5, -0.5) * scale;
	vertices[7] = glm::vec3( 0.5,  0.5,  0.5) * scale;
}

glm::vec3 BoundingBox::checkCollisionWith(BoundingBox * other)
{

	glm::mat4 tModel; //This objects model
	glm::mat4 oModel; //Other objects model

	glm::vec3 zerovec = glm::vec3(0, 0, 0); //Vector of zero size

	//If the two boxes are too far apart to collide, they are not colliding
	if (glm::length(position - other->position) > glm::length(scale)/2 + glm::length(other->scale) / 2)
		return zerovec;

	tModel = glm::rotate(tModel, rotation.x, glm::vec3(1, 0, 0));
	tModel = glm::rotate(tModel, rotation.y, glm::vec3(0, 1, 0));
	tModel = glm::rotate(tModel, rotation.z, glm::vec3(0, 0, 1));

	oModel = glm::rotate(oModel, other->rotation.x, glm::vec3(1, 0, 0));
	oModel = glm::rotate(oModel, other->rotation.y, glm::vec3(0, 1, 0));
	oModel = glm::rotate(oModel, other->rotation.z, glm::vec3(0, 0, 1));

	glm::vec3 normals[6] = {
		glm::vec4(1, 0, 0, 1) * tModel,
		glm::vec4(0, 1, 0, 1) * tModel,
		glm::vec4(0, 0, 1, 1) * tModel,
		glm::vec4(1, 0, 0, 1) * oModel,
		glm::vec4(0, 1, 0, 1) * oModel,
		glm::vec4(0, 0, 1, 1) * oModel
	};


	glm::vec3 tPos, oPos; //Both objects position projection onto the normal vector
	glm::vec3 tMin, tMax; //This objects min/max position projections
	glm::vec3 tMinVec, tMaxVec; //Vectors that point this objects to min/max points
	glm::vec3 oMin, oMax; //Other objects min/max position projections
	glm::vec3 oMinVec, oMaxVec; //Vectors that point the other objects to min/max points
	glm::vec3 vector;	  //Vector to do math on

	GLfloat minOverlap = glm::length(position - other->position) * 2; //Keeps track of the minimum overlap of the shapes
	glm::vec3 minOverlapVector;

	for (glm::vec3 normal : normals) {
		tPos = glm::proj(position, normal);
		tMin = tPos;
		tMax = tPos;
		for (glm::vec3 vert : vertices) {
			vector = glm::proj(glm::vec3(glm::vec4(vert, 1.0f) * tModel) + position, normal);
			if (sinLength(vector) < sinLength(tMin)) {
				tMin = vector;
				tMinVec = vert;
			}
			else if (sinLength(vector) > sinLength(tMax)) {
				tMax = vector;
				tMaxVec = vert;
			}
		}

		oPos = glm::proj(other->position, normal);
		oMin = oPos;
		oMax = oPos;
		for (glm::vec3 vert : other->vertices) {
			vector = glm::proj(glm::vec3(glm::vec4(vert, 1.0f) * oModel) + other->position, normal);
			if (sinLength(vector) < sinLength(oMin)) {
				oMin = vector;
				oMinVec = vert;
			}
			else if (sinLength(vector) > sinLength(oMax)) {
				oMax = vector;
				oMaxVec = vert;
			}
		}

		if (sinLength(tPos) < sinLength(oPos)) {
			if (sinLength(tMax) < sinLength(oMin))
				return zerovec;

			GLfloat overlap = glm::length(tMax - oMin);
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minOverlapVector = normal;
			}
		}
		else {
			if (sinLength(oMax) < sinLength(tMin))
				return zerovec;
			GLfloat overlap = glm::length(oMax - tMin);
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minOverlapVector = normal;
			}
		}
	}

	return minOverlapVector * minOverlap;
}

void BoundingBox::setScale(glm::vec3 _scale)
{
	if (staticBox)
		return;

	for (int x = 0; x < 8; x++) {
		vertices[x] *= _scale / scale;
	}
	scale = _scale;
}

glm::vec3 BoundingBox::getScale()
{
	return scale;
}

GLfloat BoundingBox::sinLength(glm::vec3 vec)
{
	GLfloat len = glm::length(vec);
	if (vec.x <= 0 && vec.y <= 0 && vec.z <= 0)
		return len * -1.0;
	return len;
}

GLfloat BoundingBox::sinLength(glm::vec4 vec)
{
	GLfloat len = glm::length(glm::vec3(vec.x, vec.y, vec.z));
	if (vec.x <= 0 && vec.y <= 0 && vec.z <= 0)
		return len * -1.0;
	return len;
}

GLfloat BoundingBox::maxWidth()
{
	return sqrtf(powf(scale.x, 2)
		+ powf(scale.y, 2)
		+ powf(scale.z, 2));
}

void BoundingBox::update(GLfloat deltaTime)
{
	if (staticBox)
		return;
	rotation += rotVelocity * deltaTime;
	position += velocity * deltaTime;
	velocity.y -= gravity * deltaTime;
	velocity.x -= (velocity.x * friction) * deltaTime;
	velocity.z -= (velocity.z * friction) * deltaTime;
}