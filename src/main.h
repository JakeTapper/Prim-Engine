#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"
#include "shaders.h"
#include "GameObject.h"
#include "GameScript.h"
#include "nonlib/cube.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")

GLuint program;
Camera* camera;
int keys[1024];


GLFWwindow* initGlfw(int screenWidth, int screenHeight);
void initGL(GLFWwindow* window);
void loop(GLFWwindow* window, OList* workspace);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keypress(GLfloat delta, GLFWwindow* window);
void cameraMove(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void setCamera(Camera* cam);