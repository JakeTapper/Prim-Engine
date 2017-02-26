/*
Jake Tapper
2/25/17
Framework for how every object should be made and how they should act
*/

#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GameScript.h"

class GameObject {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;

	int ID;

	GameObject(glm::vec3 _position, glm::vec3 _rotation, GLfloat _program, GLuint buffers[3]) {
		position = _position;
		rotation = _rotation;
		program = _program;

		scripts = new SList(); //Creates the linked list of script objects
		velocity = glm::vec3(0, 0, 0); //Sets the initial velocity to 0
		VAO = buffers[0];
		VBO = buffers[1];
		EBO = buffers[2];
	}

	//Gets the time since last call and updates position before calling the scripts
	virtual void update(GLfloat deltaTime) {
		position += (velocity * deltaTime);
		scripts->run();
	}

	//To be called every loop to render the object
	virtual void render(Camera* camera, GLfloat deltaTime) = 0;

protected:
	GLfloat* vertices;		//Array of verices
	GLuint* indices;		//Array of indices
	GLuint VAO, VBO, EBO;	//Buffers for rendering
	GLfloat program;		//GL program to use
	SList* scripts;			//Linked list of scripts
};

class OList {
	struct Node {
		Node* last;
		Node* next;
		GameObject* data;
	};

public:
	Node* dummy;

	OList() {
		dummy = new Node();
		dummy->last = 0;
		dummy->next = 0;
		dummy->data = 0;
	}

	//Adds a GameObject to the end of the list
	void append(GameObject* g) {
		Node* p = dummy;
		int pos = -1;
		while (p->next) {
			p = p->next;
			pos++;
		}
		p->next = new Node();
		p->next->last = p;
		p->next->data = g;
		p->next->next = 0;

		p->next->data->ID = pos;
	}

	//Calls every objects render function
	void render(Camera* camera, GLfloat deltaTime) {
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->render(camera, deltaTime);
		}
	}

	//Calls every objects update function
	void update(GLfloat deltaTime) {
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->update(deltaTime);
		}
	}

private:
	//Node* dummy;
};