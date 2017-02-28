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
#include <memory>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>


#include "camera.h"
#include "GameObject.h"
#include "GameScript.h"
#include "Shader.h"
#include "nonlib/cube.h"
#include "nonlib/playerCube.h"


#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "BulletDynamics_vs2010_x64_debug")
#pragma comment(lib, "BulletCollision_vs2010_x64_debug")
#pragma comment(lib, "LinearMath_vs2010_x64_debug")


Shader* program;		//Shader program compiled from the files
Camera* camera;			//Camera object to handle mouse movment and key movement
int keys[1024];			//Holds key inputs
GLuint VBO, VAO, EBO;	//Open GL buffers
btBroadphaseInterface* broadphase;							//Physics broadphase
btDefaultCollisionConfiguration* collisionConfiguration;	//Physics collision config
btCollisionDispatcher* dispatcher;							//Physics collisions dispatcher
btSequentialImpulseConstraintSolver* solver;				//Collisions solver
btDiscreteDynamicsWorld* world;								//Physics world
PlayerCube* player;											//Player object to track with camera and move


//Starts GLFW and sets version, as well as creating the window
GLFWwindow* initGlfw(int screenWidth, int screenHeight);
//Starts open GL and initiates buffers and the GL program
void initGL(GLFWwindow* window);
//Creates the initial states for Bullet physics engine
void initBullet();
//The main game loop that calls the render() and update() functions in every object in workspace
void loop(GLFWwindow* window, OList* workspace, int maxFps);
//The callback function for the GLFW window. Sets the key boolean in keys[] when pressed or released
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Handles the actual keypress by calling the proper Camera function
void keypress(GLfloat delta, GLFWwindow* window);
//Callback function for mouse input. Calls the Camera mouseMove() function
void cameraMove(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
//Sets the global camera variable
void setCamera(Camera* cam);
//Sleeps the the corrent amount of time
void sleep(unsigned millis);