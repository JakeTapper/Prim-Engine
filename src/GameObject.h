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

	GameObject(glm::vec3 _position, glm::vec3 _rotation, GLfloat _program) {
		position = _position;
		rotation = _rotation;
		program = _program;

		scripts = new SList();
		velocity = glm::vec3(0, 0, 0);
	}

	virtual void update() {
		GLfloat time, deltaTime;

		time = glfwGetTime();
		deltaTime = time - lastRun;
		lastRun = time;

		position += (velocity * deltaTime);
		scripts->run();
	}

	virtual void render(Camera* camera, GLuint* buffers) = 0;

protected:
	GLfloat* vertices;
	GLuint* indices;
	GLfloat program;
	SList* scripts;

private:
	GLfloat lastRun;
};

class OList {
	struct Node {
		Node* last;
		Node* next;
		GameObject* data;
	};

public:
	OList() {
		dummy = new Node();
		dummy->last = 0;
		dummy->next = 0;
		dummy->data = 0;
	}

	void append(GameObject* g) {
		Node* p = dummy;
		while (p->next)
			p = p->next;
		p->next = new Node();
		p->next->last = p;
		p->next->data = g;
		p->next->next = 0;
	}

	void render(Camera* camera, GLuint* buffers) {
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->render(camera, buffers);
		}
	}

	void update() {
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->update();
		}
	}

private:
	Node* dummy;
};