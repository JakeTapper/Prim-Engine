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
#include <btBulletCollisionCommon.h>

#include "GameScript.h"
#include "camera.h"

class OList;
class GameObject;

class GameObject {
public:
	glm::vec3 position; //Object position in the world space
	glm::vec3 rotation; //Object rotational state
	glm::vec3 velocity; //Current velocity
	glm::vec3 rotveloc; //Rotational velocity
	glm::vec3 size;		//Size in multiples of a base size object
	GLfloat   mass;		//Mass of the object. Set based on size and ratio of 2.5:1 mass to size
	btRigidBody* body;  //Rigidbody of the object

	int ID; //Unique ID for each OList. Assigned by the list itself when appended

	GameObject() {} //Default constructor

	GameObject(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _size, GLfloat _program, GLuint buffers[3], btDiscreteDynamicsWorld* world, GLfloat _mass) {
		position = _position;
		rotation = _rotation;
		program = _program;
		size = _size;
		mass = _mass;

		scripts = new SList(); //Creates the linked list of script objects
		velocity = glm::vec3(0, 0, 0); //Sets the initial velocity to 0
		VAO = buffers[0];
		VBO = buffers[1];
		EBO = buffers[2];

		//Creates the bounding box for the object
		btCollisionShape* box = new btBoxShape(btVector3(size.x/2, size.y/2, size.z/2));
		//Sets the rotation and position for the rigid body
		btDefaultMotionState* state = new btDefaultMotionState(btTransform(
			btQuaternion(rotation.x, rotation.y, rotation.z, 1),
			btVector3(position.x, position.y, position.z)
		));
		//Constructs the rigidbody
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                   //Mass (0 = static)
			state,					//State
			box,					//Collision shape
			btVector3(0, 0, 0)		//Inertia
		);
		//Sets the object rigidbody to the newly created one
		body = new btRigidBody(rigidBodyCI);
		//Adds the cube to the physics world
		world->addRigidBody(body);
	}

	//Gets the time since last call and updates position before calling the scripts
	virtual void update(GLfloat deltaTime) {
		//Runs all of the scripts (Nothing as of yet)
		scripts->run();
		//Sets the position and rotation to what the physics engine has determined
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		rotation = glm::vec3(body->getOrientation().getX(), body->getOrientation().getY(), body->getOrientation().getZ());
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
		int pos;
	};

public:
	Node* dummy;		//Dummy node at the beggining of the list
	GameObject** arr;	//Array version of the list
	int size;			//Size of the array/list

	OList() {
		arr = 0; //Initilizes array to null
		//Creates the dummy and zeros the data in it
		dummy = new Node(); 
		dummy->last = 0;
		dummy->next = 0;
		dummy->data = 0;
		dummy->pos = 0;
	}

	//Adds a GameObject to the end of the list
	void append(GameObject* g) {
		Node* p = dummy;
		while (p->next) 
			p = p->next;
		p->next = new Node();
		p->next->last = p;
		p->next->data = g;
		p->next->next = 0;
		p->next->pos = p->pos + 1;
		p->next->data->ID = p->next->pos;
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

		size = length();
		arr = toArr();
		
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->update(deltaTime);
		}
	}

	//Gets the length of the list
	int length() {
		Node* p = dummy;
		int len = 0;
		while (p->next) {
			p = p->next;
			len++;
		}
		return len;
	}

	//Converts the list to an array and returns it
	GameObject** toArr() {
		if (arr) free(arr);
		GameObject** ret = (GameObject**) malloc(sizeof(GameObject*) * (size + 1));
		if (!ret) exit(-4);

		Node* p = dummy;
		int pos = -1;
		while (1) {
			if (!p->next) break;
			p = p->next;
			pos++;

			ret[pos] = p->data;
		}
		ret[size] = 0;
		return ret;
	}

	void clearList(btDynamicsWorld* world) {
		Node* p = dummy;
		while (p->next)
			p = p->next;
		while (p != dummy) {
			p = p->last;
			free(p->next->data);
			free(p->next);
		}
		free(dummy);
	}
};