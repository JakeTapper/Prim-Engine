/*
Jake Tapper
2/25/17
Simple script for having an object fall at 9.81 m/s
*/

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

		parent->velocity += glm::vec3(0.0, -9.81 * deltaTime, 0.0);
	}
private:
	GLdouble lastRun;
};