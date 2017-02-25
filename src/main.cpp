/*
Jake Tapper
2/24/17
*/

#include "main.h"

GLuint VBO, VAO, EBO;

int main() {
	OList* workspace = &OList();
	GLFWwindow* window = initGlfw(1280, 720);
	
	initGL(window);
	setCamera(&Camera(5.0f, program, window, 100));

	for (float x = -1.5; x <= 1.5; x += 1.5) {
		for (float y = -1.5; y <= 1.5; y += 1.5) {
			for (float z = -1.5; z <= 1.5; z += 1.5) {
				workspace->append((GameObject*) new Cube(glm::vec3(x, y, z), glm::vec3(0,0,0), program));
			}
		}
	}

	loop(window, workspace, 60);
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

	//Sets the callback function and cursor mode
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

	//Loads the shaders that are in shaders.h and compiles them to the program
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glShaderSource(frag, 1, &fragmentCode, NULL);
	glCompileShader(vertex);
	glCompileShader(frag);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(frag);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void loop(GLFWwindow* window, OList* workspace, int fpsTarget) {
	GLfloat currentFrame, deltaTime = 0, lastFrame = 0;
	GLuint buffers[3] = { VBO, VAO, EBO };
	
	while (!glfwWindowShouldClose(window)) {
	
		//Waits until the proper interval so the FPS target is hit
		while (glfwGetTime() - lastFrame < (1.0 / fpsTarget))
			sleep(25);			

		//Gets time since last loop
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Gets user input
		glfwPollEvents();
		keypress(deltaTime, window);

		//Render the frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Calls the update and render functions of the camera and game objects/scripts
		camera->update();
		workspace->update();
		workspace->render(camera, buffers);		
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS)
		keys[key] = 1;
	else if (action == GLFW_RELEASE)
		keys[key] = 0;
}

void keypress(GLfloat delta, GLFWwindow* window) {
	if (keys[GLFW_KEY_W])
		camera->w();
	if (keys[GLFW_KEY_S])
		camera->s();
	if (keys[GLFW_KEY_A])
		camera->a();
	if (keys[GLFW_KEY_D])
		camera->d();

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