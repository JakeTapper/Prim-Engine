#pragma once
#include "..\GameObject.h"

class Gravity :GameScript {
public:
	Gravity(GameObject* _parent) : GameScript(_parent) {
		lastRun = glfwGetTime();
	}

	void run() {
		GLdouble time, deltaTime;
		
		time = glfwGetTime();
		deltaTime = time - lastRun;
		lastRun = time;

		parent->velocity += glm::vec3(0.0, -9.8 * deltaTime, 0.0);
	}
private:
	GLdouble lastRun;
};