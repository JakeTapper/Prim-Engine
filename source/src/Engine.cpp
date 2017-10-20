#include "Engine.h"

Camera* cam;
Engine* GameObject::engine;
int* keys;

Engine::Engine()
{
}

Engine::~Engine()
{
	delete camera;
}

void Engine::run()
{
	try {
		gameLoop();
	}
	catch (const std::runtime_error& e) {
		printf("Runtime error!\n%s\n", e.what());
	}
}

void Engine::init(const char * title, int width, int height, const char* vertexFile, const char* fragmentFile, glm::vec3 _clearColor)
{
	try {
		//GLFW init
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, 0);

		clearColor = _clearColor;

		window = glfwCreateWindow(width, height, title, 0, 0);
		if (!window) {
			throw std::runtime_error("Failed to create GLFW window\n");
		}
		glfwMakeContextCurrent(window);


		//GLEW init
		glewExperimental = 1;
		if (glewInit()) {
			throw std::runtime_error("Failed to init GLEW\n");
		}

		int bufferWidth, bufferHeight;
		glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
		glViewport(0, 0, bufferWidth, bufferHeight);
		glEnable(GL_DEPTH_TEST);

		GLuint projLoc = glGetUniformLocation(program, "projection");
		glm::mat4 proj = glm::perspective(glm::radians(90.0f / 2), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		std::vector<char> vertexCode = readFile(vertexFile);
		std::vector<char> fragmentCode = readFile(fragmentFile);

		const char* vertexString = &vertexCode[0];
		const char* fragmentString = &fragmentCode[0];

		glShaderSource(vertex, 1, &vertexString, NULL);
		glShaderSource(frag, 1, &fragmentString, NULL);
		glCompileShader(vertex);
		glCompileShader(frag);

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, frag);
		glLinkProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(frag);

		camera = new Camera(0.5, program, window, 90);
		cam = camera;

		glUseProgram(program);
		GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
		GLint lightColorLoc = glGetUniformLocation(program, "lightColor");
		glUniform3f(lightPosLoc, -10, 15, -10);
		glUniform3f(lightColorLoc, 1, 1, 1);

		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, &keyCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouseMove);

		//Lua init
		luaState = luaL_newstate();
		setupLuaState(luaState);
		luabridge::setGlobal(luaState, this, "Engine");

		GameObject::engine = this;
		keys = new int[1024];
		
		int luaReturn = luaL_dofile(luaState, "gamedata/main.lua");
		luabridge::LuaRef luaSetup = luabridge::getGlobal(luaState, "setup");
		if (luaSetup.isFunction()) {
			luaSetup();
		}
		else {
			printf("%s\n", "Lua setup is not a function");
		}
		if (luaReturn) {
			printf("%s\n", lua_tostring(luaState, luaReturn));
			lua_pop(luaState, 1);
		}
	}
	catch (const std::runtime_error& e) {
		printf("Runtime error!\n%s\n", e.what());
		exit(-1);
	}
}

void Engine::setupLuaState(lua_State* _luaState) {
	luaL_openlibs(_luaState);

	luabridge::getGlobalNamespace(_luaState)
		.beginNamespace("engine")
			.beginClass <Engine>("Engine")
				.addFunction("getObject", &Engine::getObject)
			.endClass()
		.endNamespace();

	GameObject::addToLua(_luaState);
	Cube::addToLua(_luaState);
}

void Engine::addObject(GameObject * object)
{
	objects.push_back(object);
}

GLuint Engine::getProgram()
{
	return program;
}

Camera* Engine::getCamera()
{
	return camera;
}

void Engine::gameLoop()
{
	GameObject* player = getObject("Player");

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Sets the camera to the GameObject to the name of "Player" or to 0,0,0
		if (player) {
			camera->update(&(player->getPosition()));
		}
		else {
			camera->update( &glm::vec3(0,0,0) );
		}

		updateObjects();
		renderObjects();
		checkForCollisions();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void Engine::renderObjects() {
	for (GameObject* o : objects) {
		o->render();
	}
}

void Engine::updateObjects() {
	for (GameObject* o : objects) {

		//If the object has a lua script, pass it the list of keys
		if (o->scriptState) {
			luabridge::LuaRef keysPressed = luabridge::newTable(o->scriptState);
			for (int x = 1; x < 1024; x++) {
				keysPressed[x] = keys[x];
			}
			luabridge::setGlobal(o->scriptState, keysPressed, "keys");
		}

		o->update();
	}
}

void Engine::checkForCollisions()
{
	GameObject* object0; //First object to check for collisions
	GameObject* object1; //Second object to check for collisions
	glm::vec3 collided; //Holds the vector of the collision between the objects
	glm::vec3 normalizedCollision; //Normalized projection of the object's relative position projected onto the collision vector

	for (int x = 0; x < objects.size(); x++) {
		for (int y = x + 1; y < objects.size(); y++) {
			object0 = objects.at(x);
			object1 = objects.at(y);

			//If both objects are static, there's no reason to check them for collisions
			if (object0->getBoundingBox()->staticBox && object1->getBoundingBox()->staticBox)
				continue;

			collided = object0->getBoundingBox()->checkCollisionWith(object1->getBoundingBox());
			if (glm::length(collided)) {

				//Moves the objects away from each other and changes their velocity
				if (!(object0->getBoundingBox()->staticBox)) {
					normalizedCollision = glm::normalize(glm::proj(object0->getPosition() - object1->getPosition(), collided));
					object0->getBoundingBox()->position += glm::length(collided) * normalizedCollision;

					if (object1->getBoundingBox()->staticBox) {
						object0->getBoundingBox()->velocity -= glm::proj(object0->getBoundingBox()->velocity, collided);
					}
					else {
						object0->getBoundingBox()->velocity -= (glm::proj(object0->getBoundingBox()->velocity, collided) - glm::proj(object1->getBoundingBox()->velocity, collided)) / (GLfloat)2;
					}
				}

				if (!(object1->getBoundingBox()->staticBox)) {
					normalizedCollision = glm::normalize(glm::proj(object1->getPosition() - object0->getPosition(), collided));
					object1->getBoundingBox()->position += glm::length(collided) * normalizedCollision;

					if (object0->getBoundingBox()->staticBox) {
						object1->getBoundingBox()->velocity -= glm::proj(object1->getBoundingBox()->velocity, collided);
					}
					else {
						object1->getBoundingBox()->velocity -= (glm::proj(object1->getBoundingBox()->velocity, collided) - glm::proj(object0->getBoundingBox()->velocity, collided)) / (GLfloat)2;
					}
				}

				//Calls the onCollide functions for the object's lua scripts
				if (object0->scriptState) {
					luabridge::LuaRef collisionFunction = luabridge::getGlobal(object0->scriptState, "onCollide");
					if (collisionFunction.isFunction()) {
						collisionFunction((Cube*)object1);
					}
				}
				if (object1->scriptState) {
					luabridge::LuaRef collisionFunction = luabridge::getGlobal(object1->scriptState, "onCollide");
					if (collisionFunction.isFunction()) {
						collisionFunction((Cube*)object0);
					}
				}
			}
		}
	}
}

std::vector<char> Engine::readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

GameObject* Engine::getObject(std::string objectName)
{
	for (GameObject* obj : objects) {
		if (!objectName.compare((obj)->name)) {
			return obj;
		}
	}
	return 0;
}

void Engine::loadLuaScript(const char* scriptName, GameObject* gameobject) {
	gameobject->scriptState = luaL_newstate();
	Engine::setupLuaState(gameobject->scriptState);
	luaL_dofile(gameobject->scriptState, scriptName); 

	luabridge::LuaRef setup = luabridge::getGlobal(gameobject->scriptState, "setup");
	if (setup.isFunction()) {
		setup();
	}
}

void mouseMove(GLFWwindow * window, GLdouble xpos, GLdouble ypos)
{
	cam->mouseMove(window, xpos, ypos);

}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, 1);
	}
	else if (action == GLFW_PRESS) {
		keys[key] = 1;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = 0;
	}

}