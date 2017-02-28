#pragma once
#include "GameObject.h"
#include "camera.h"

class PlayerCube : GameObject {
public:

	btRigidBody* body;		//Rigidbody for physics simulation
	glm::vec3 position;		//Keeps track of position for rendering
	glm::vec3 lastPos;		//Keeps track of last position for calculating velocity
	int inAir, falling;		//Keeps track of jumping and falling to find if the player can jump when space is pressed

	PlayerCube(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _size, GLfloat _program, GLuint buffers[3], btDiscreteDynamicsWorld* world, int _mass) {
		//Sets variables
		program = _program;
		size = _size;
		inAir = 0;
		falling = 0;
		position = _position;
		lastPos = _position;

		glm::vec3 color = glm::vec3(.2, .2, 1);

		scripts = new SList(); //Creates the linked list of script objects
		velocity = glm::vec3(0, 0, 0); //Sets the initial velocity to 0
		VAO = buffers[0];
		VBO = buffers[1];
		EBO = buffers[2];
		
		//Creates the bounding box for the cube
		btCollisionShape* shape = new btBoxShape(btVector3(size.x/2, size.y/2, size.z/2));
		//Sets the rotation and position for the rigid body
		btDefaultMotionState* state =
			new btDefaultMotionState(btTransform(btQuaternion(_rotation.x, _rotation.y, _rotation.z, 1), btVector3(_position.x, _position.y, _position.z)));
		//Sets the mass and inertia
		btScalar mass = _mass;
		btVector3 inertia(0, 0, 0);
		shape->calculateLocalInertia(mass, inertia);
		//Constructs the rigidbody
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, state, shape, inertia);
		btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
		//Sets the cube rigidbody to the newly created one
		body = rigidBody;
		//Adds the cube to the physics world
		world->addRigidBody(rigidBody);
		//Disabled deactivation due to object inactivity
		body->setActivationState(DISABLE_DEACTIVATION);
		//Stops the cube from rotating
		body->setAngularFactor(btVector3(0, 0, 0));


		//Allocates memory for the verices and indices arrays
		vertices = (GLfloat*)malloc(sizeof(GLfloat) * 324);
		if (!vertices) exit(-3);

		//Creates the array of verices and copies it to the objects vertices array
		GLfloat v[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0, 0, 0,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0, 0, 0,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0, 0, 0,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0, 0, 0,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0, 0, 0,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0, 0, 0
		};
		//Changes the color (the last 3 numbers on each line - all zeros at init) of the vertices
		for (int x = 6; x < 322; x += 9) {
			v[x] = color.x;
			v[x + 1] = color.y;
			v[x + 2] = color.z;
		}
		//Scales the cube
		for (int x = 0; x < 324; x += 9) {
			v[x] *= size.x;
			v[x + 1] *= size.y;
			v[x + 2] *= size.z;
		}
		//Copies the vertices data to the cubes vertices variable
		for (int x = 0; x < 324; x++) {
			vertices[x] = v[x];
		}
	}

	//Renders the object to the window
	void render(Camera* camera, GLfloat deltaTime) {
		//Creates local arrays for the verices and indices so that they are
		//properly read by the GPU buffers
		GLfloat verts[324];
		memcpy(verts, vertices, 324 * sizeof(GLfloat));

		//Bind vertex array buffer
		glBindVertexArray(VAO);

		//Set up VBO and pass data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		//Set up VAO format
		//Position  
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Unit normal vector
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//Color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		glUseProgram(program);

		//Get uniform locations
		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(program, "model");
		GLint viewPosLoc = glGetUniformLocation(program, "viewPos");

		//Move and rotate the cube model, then set uniforms
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(viewPosLoc, camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

		//Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	virtual void update(GLfloat deltaTime) {	
		//Runs all of the scripts (Nothing as of yet)
		scripts->run();
		//Sets the position and rotation to what the physics engine has determined
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		rotation = glm::vec3(body->getOrientation().getX(), body->getOrientation().getY(), body->getOrientation().getZ());


		glm::vec3 motion = position - lastPos;
		if (inAir && !falling && motion.y < 0) //Apex is reached when velocity is lower than 0
			falling = 1;
		else if (falling && motion.y >= 0) { //If it was falling, and is not at rest, it has hit the ground
			inAir = 0;
			falling = 0;
		}
		lastPos = position;
	}

	//Moves based on the current keypresses
	void movement(int keys[], Camera* camera) {
		if (keys[GLFW_KEY_W])
			body->applyCentralForce(conv(camera->cameraFront, 5));
		if (keys[GLFW_KEY_A])
			body->applyCentralForce(conv(glm::cross(camera->cameraFront, camera->cameraUp), -5));
		if (keys[GLFW_KEY_D])
			body->applyCentralForce(conv(glm::cross(camera->cameraFront, camera->cameraUp), 5));
		if (keys[GLFW_KEY_S])
			body->applyCentralForce(conv(camera->cameraFront, -5));
		if (keys[GLFW_KEY_SPACE] && !inAir) {
			body->applyCentralForce(btVector3(0, 150, 0));
			inAir = 1;
		}
			
	}

	//Helper functions to convert from GLM vector3s and btVector3s
	btVector3 conv(glm::vec3 v) {
		return btVector3(v.x, v.y, v.z);
	}

	//Scales the vector by n while converting
	btVector3 conv(glm::vec3 v, int n) { 
		return btVector3(v.x * n, v.y * n, v.z * n);
	}
};