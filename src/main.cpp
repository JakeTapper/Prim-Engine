/*
Jake Tapper
2/28/17
*/

#include "main.h"

//Testing function. For standalone game, delete this and call the init functions and loop() as done here
int main() {
	//Linked list of all game objects to be rendered
	OList* workspace = &OList();
	//GLFW window
	GLFWwindow* window = initGlfw(1280, 720);
	initGL(window);
	//Player camera
	setCamera(&Camera(5.0f, program->program, window, 100));
	//OpenGL buffer array to pass to new objects
	GLuint* buffers =  new GLuint[3] {VAO, VBO, EBO};

	//Creates physics simulation
	initBullet();

	//Ground block
	workspace->append((GameObject*) new Cube(glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(20, 1, 20), glm::vec3(.1, 1, .1), program->program, buffers, world, 0));

	//Static blocks
	workspace->append((GameObject*) new Cube(glm::vec3(3, -1.25, 2), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(0, .1, 1), program->program, buffers, world, 0));
	workspace->append((GameObject*) new Cube(glm::vec3(5, -1.25, -4), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(0, .1, 1), program->program, buffers, world, 0));
	workspace->append((GameObject*) new Cube(glm::vec3(-3, -1.25, -6), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(0, .1, 1), program->program, buffers, world, 0));

	//Dynamic blocks
	workspace->append((GameObject*) new Cube(glm::vec3(-3, -1.25, -2), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(.7, .1, .1), program->program, buffers, world, .125));
	workspace->append((GameObject*) new Cube(glm::vec3(-5, -1.25, 4), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(.7, .1, .1), program->program, buffers, world, .125));
	workspace->append((GameObject*) new Cube(glm::vec3(3, -1.25, 6), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(.7, .1, .1), program->program, buffers, world, .125));
	workspace->append((GameObject*) new Cube(glm::vec3(-3, -1.25, 6), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), glm::vec3(.7, .1, .1), program->program, buffers, world, .125));

	//Start the game loop
	loop(window, workspace, 60);
	
	//Delete buffers
	delete[] buffers;
}

GLFWwindow* initGlfw(int screenWidth, int screenHeight) {

	//Starts glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	
	//Creates the GLFW window itself
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Window", 0, 0);

	//Checks to be sure that the window has been properly created
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}

	//Sets the callback functions and cursor mode
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cameraMove);

	return window;
}

void initGL(GLFWwindow* window) {
	//Starts glew then checks to be sure that it properly started
	glewExperimental = 1;
	if (glewInit()) {
		printf("Failed to init GLEW\n");
		exit(-2);
	}

	//Gets the size of the frambuffer then uses it to set the opengl viewport size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	//Loads the shaders
	program = new Shader("src/shaders/default.vertex", "src/shaders/default.frag");

	//Set light color and position
	glUseProgram(program->program);
	GLint lightPosLoc = glGetUniformLocation(program->program, "lightPos");
	GLint lightColorLoc = glGetUniformLocation(program->program, "lightColor");
	glUniform3f(lightPosLoc, 0, 5, 0);
	glUniform3f(lightColorLoc, 1, .2, .2);

	//Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void initBullet() {
	//Broadphase
	broadphase = new btDbvtBroadphase();

	//Collision detector and dispatchet
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//Collision solver
	solver = new btSequentialImpulseConstraintSolver;

	//Create physics world
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->setGravity(btVector3(0, -9.81, 0));
}

void loop(GLFWwindow* window, OList* workspace, int maxFps) {
	GLfloat currentFrame, deltaTime = 0, lastFrame = 0;
	GLuint* buffers = new GLuint[3]{ VAO, VBO, EBO };

	player = new PlayerCube(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(.5, .5, .5), program->program, buffers, world, 1);
	workspace->append((GameObject*) player);
	
	while (!glfwWindowShouldClose(window)) {
	
		//Waits until the proper interval so the maxFps isn't exceeded
		while (glfwGetTime() - lastFrame < (1.0 / maxFps))
			sleep(10);			


		//Gets time since last loop
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//Step physics by time changed
		world->stepSimulation(deltaTime, 10);

		//Gets user input
		glfwPollEvents();
		keypress(deltaTime, window);

		//Render the frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calls the update and render functions of the camera and game objects/scripts
		camera->update(player->position);
		workspace->update(deltaTime);
		workspace->render(camera, deltaTime);
		
		//Swaps buffers
		glfwSwapBuffers(window);
	}

	//Clean up pointers
	delete[] buffers;

	int i;
	for (i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
		delete obj;
	}
	workspace->clearList(world);
	delete world;

	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete program;

	//Close window
	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS)
		keys[key] = 1;
	else if (action == GLFW_RELEASE)
		keys[key] = 0;
}

void keypress(GLfloat delta, GLFWwindow* window) {
	player->movement(keys, camera);
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, 1);
}

void cameraMove(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
	camera->mouseMove(window, xpos, ypos);
}

void setCamera(Camera* cam) {
	camera = cam;
}

//Detects OS type and calls the appropriate sleep function
#ifdef _WIN32
#include <windows.h>

void sleep(unsigned millis) {
	Sleep(millis);
}

#else 
#include <unistd.h>

void sleep(unsigned millis) {
	usleep(millis * 1000);
}

#endif
