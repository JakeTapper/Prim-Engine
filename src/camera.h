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
		proj = glm::perspective(glm::radians(fov / 2), (float)width / (float)height, 0.1f, 100.0f);
	}

	//Handles user mouse movement
	void mouseMove(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
		//Sets first mouse movement. Causes 90 degree shift at beginning if not used
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		//Detects amount that the mouse has moved
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		//Mouse sensitvity
		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		//Sets the yaw and pitch of the movement
		yaw += xoffset;
		pitch += yoffset;

		//Keeps pitch within the 90 degree boundry
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		//Determines the vector pointing forward from the camera
		glm::vec3 front = glm::vec3(0, 0, 0);
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	//Called every loop to set the shaders based on camera position and direction
	void update(glm::vec3 position) {
		//Locks camera position to that of the PlayerCube position
		cameraPos = position + glm::vec3(0, .5, 0) - (cameraFront * glm::vec3(5));
		//Detemines the camera directions
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(position - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		//Points the camera at the correct location
		glm::mat4 view = glm::lookAt(cameraPos, position + cameraFront, cameraUp);

		//Sets the uniforms for camera view and projection
		GLint viewLoc, projLoc;

		viewLoc = glGetUniformLocation(program, "view");
		projLoc = glGetUniformLocation(program, "projection");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
};