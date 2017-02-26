/*
Jake Tapper
2/25/17
A basic 1x1x1 cube that is colored differently in each corner for easier
recognition
*/

#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "..\GameObject.h"
#include "Gravity.h"

class Cube :GameObject {
public:
	Cube(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 color, GLfloat _program, GLuint buffers[3])
		: GameObject(_position, _rotation, _program, buffers) {

		//Allocates memory for the verices and indices arrays
		vertices = (GLfloat*)malloc(sizeof(GLfloat) * 324);
		if (!vertices) exit(-3);

		//Creates the array of verices and copies it to the objects vertices array
		GLfloat v[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0
		};
		for (int x = 6; x < 322; x += 9) {
			v[x] = color.x;
			v[x + 1] = color.y;
			v[x + 2] = color.z;
		}

		for (int x = 0; x < 324; x++) {
			vertices[x] = v[x];
		}
	}

	void render(Camera* camera, GLfloat deltaTime) {
		//Creates local arrays for the verices and indices so that they are
		//properly read by the GPU buffers
		GLfloat verts[324];
		memcpy(verts, vertices, 324 * sizeof(GLfloat));

		//Bind vertex array buffer
		glBindVertexArray(VAO);

		//Set up VBO and pass data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		//Set up VAO format
		//Position  
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Unit normal vector
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//Color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		glUseProgram(program);

		//Get uniform locations
		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(program, "model");
		GLint viewPosLoc = glGetUniformLocation(program, "viewPos");

		//Move and rotate the cube model, then set uniforms
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(viewPosLoc, camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		
		//Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void update(GLfloat deltaTime) {
		rotation.x += glm::radians(.3);
		rotation.y += glm::radians(.3);
		rotation.z += glm::radians(.3);

		GameObject::update(deltaTime);
	}
};