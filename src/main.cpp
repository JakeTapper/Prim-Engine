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
	setCamera(&Camera(5.0f, program, window));

	for (float x = -1.5; x <= 1.5; x += 1.5) {
		for (float y = -1.5; y <= 1.5; y += 1.5) {
			for (float z = -1.5; z <= 1.5; z += 1.5) {
				workspace->append((GameObject*) new Cube(glm::vec3(x, y, z), glm::vec3(0,0,0), program));
			}
		}
	}

	loop(window, workspace);
}

GLFWwindow* initGlfw(int screenWidth, int screenHeight) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Window", 0, 0);

	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cameraMove);

	return window;
}

void initGL(GLFWwindow* window) {
	glewExperimental = 1;

	if (glewInit()) {
		printf("Failed to init GLEW\n");
		exit(-2);
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

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

void loop(GLFWwindow* window, OList* workspace) {
	GLfloat currentFrame, deltaTime = 0, lastFrame = 0;
	GLuint buffers[3] = { VBO, VAO, EBO };
	
	while (!glfwWindowShouldClose(window)) {

		while (glfwGetTime() - lastFrame < 0.016667);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		printf("%lf\n", deltaTime);

		glfwPollEvents();
		keypress(deltaTime, window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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