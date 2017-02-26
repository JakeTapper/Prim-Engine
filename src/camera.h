#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class Camera {
public:
	//Projection for where objects should be rendered
	glm::mat4 proj;
	//Camera directions/positions
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraTarget;
	glm::mat4 view;

	//Transveral speed of the camera
	GLfloat cameraSpeed;
	//Last mouse x and y coordinates
	GLfloat lastX, lastY;
	//Current pitch and yaw of the camera
	GLfloat pitch, yaw;
	//Keep track of time since last frame
	GLfloat deltaTime, lastFrame, currentFrame;
	//OpenGL program to use
	GLuint program;

	//Allows the initial setting of camera state
	int firstMouse = true;

	//cameraSpeed, openGL program, GLFW window being used, field of view in degrees
	Camera(GLfloat speed, GLuint prog, GLFWwindow* window, GLfloat fov) {
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
		proj = glm::perspective(glm::radians(fov/2), (float)width / (float)height, 0.1f, 100.0f);
	}

	//Handles user mouse movement
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

	//Called every loop to set the shaders based on camera position and direction
	void update() {
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		GLint viewLoc, projLoc;

		viewLoc = glGetUniformLocation(program, "view");
		projLoc = glGetUniformLocation(program, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	//Correspond to user key inputs. Called by keypress() function in main.cpp
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