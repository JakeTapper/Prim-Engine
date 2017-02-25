#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class Camera {
public:
	glm::mat4 proj;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraTarget;
	glm::mat4 view;

	GLfloat cameraSpeed;
	GLfloat lastX, lastY;
	GLfloat pitch, yaw;
	GLfloat deltaTime, lastFrame, currentFrame;
	GLuint program;

	int firstMouse = true;

	Camera(GLfloat speed, GLuint prog, GLFWwindow* window) {
		cameraSpeed = speed;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		pitch = 0;
		yaw = -90;
		deltaTime = 0;
		lastFrame = 0;
		program = prog;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		proj = glm::perspective(glm::radians(50.0f), (float)width / (float)height, 0.1f, 100.0f);
	}

	void mouseMove(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	void update() {
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		GLint viewLoc = glGetUniformLocation(program, "view");
		GLint projLoc = glGetUniformLocation(program, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void w() {
		cameraPos += deltaTime * cameraSpeed * cameraFront;
	}
	void a() {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * cameraSpeed;
	}
	void s() {
		cameraPos -= deltaTime * cameraSpeed * cameraFront;
	}
	void d() {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime * cameraSpeed;
	}
};