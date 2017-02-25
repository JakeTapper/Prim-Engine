#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "..\GameObject.h"
#include "Gravity.h"

class Cube:GameObject {
public:
	Cube(glm::vec3 _position, glm::vec3 _rotation, GLfloat _program)
		: GameObject(_position, _rotation, _program) {
		
		vertices = (GLfloat*)malloc(sizeof(GLfloat) * 48);
		if (!vertices) exit(-3);
		indices = (GLuint*)malloc(sizeof(GLfloat) * 36);
		if (!indices) exit(-3);

		GLfloat v[] = {
			-0.5f , -0.5f , -0.5f , 0, 0, 0,
			-0.5 , -0.5 , 0.5 , 0, 0, 1,
			-0.5 , 0.5 , -0.5 , 0, 1, 0,
			-0.5 , 0.5 , 0.5 , 0, 1, 1,
			0.5 , -0.5 , -0.5 , 1, 0, 0,
			0.5 , -0.5 , 0.5 , 1, 0, 1,
			0.5 , 0.5 , -0.5 , 1, 1, 0,
			0.5 , 0.5 , 0.5 , 1, 1, 1
		};
		for (int x = 0; x < 48; x++) {
			vertices[x] = v[x];
		}

		GLfloat i[] = {
			0,1,3,
			0,2,3,
			0,1,5,
			0,4,5,
			0,4,6,
			0,2,6,

			7,3,2,
			7,6,2,
			7,3,1,
			7,5,1,
			7,6,4,
			7,5,4
		};
		for (int x = 0; x < 36; x++) {
			indices[x] = i[x];
		}

		scripts->append((GameScript*) new Gravity(this));
	}

	void render(Camera* camera, GLuint* buffers) {
		GLfloat verts[48];
		memcpy(verts, vertices, 48 * sizeof(GLfloat));

		GLuint inds[36];
		memcpy(inds, indices, 36 * sizeof(GLuint));


		GLuint VBO, VAO, EBO;
		VBO = buffers[0];
		VAO = buffers[1];
		EBO = buffers[2];
		//Bind vertex array buffer
		glBindVertexArray(VAO);

		//Set up VBO and EBO and pass data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

		//Set up VAO format
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glUseProgram(program);

		glm::mat4 model;

		GLint modelLoc = glGetUniformLocation(program, "model");

		glBindVertexArray(VAO);

		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void update() {
		rotation.x += glm::radians(1.0f);
		rotation.y += glm::radians(1.0f);
		rotation.z += glm::radians(1.0f);

		GameObject::update();
	}
};