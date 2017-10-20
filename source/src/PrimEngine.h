/*
Jake Tapper
3/21/17

Header file for the main part of PrimEngine. Includes all imports and function prototypes.
*/

#pragma once

//Standard header file imports
#include <cstdio>
#include <iostream>
//Library imports
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\string_cast.hpp>

//PrimEngine imports
	//Imports with no dependecies on other PrimEngine files
#include "Camera.h"
#include "BoundingBox.h"
	//Imports with dependencies on other PrimEngine files
#include "GameObject.h" //Needs BoundingBox
#include "Engine.h" //Needs GameObject and Camera
#include "Cube.h"  //Needs GameObject


//Library inclusions
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")