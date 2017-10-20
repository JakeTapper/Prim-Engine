#include "Cube.h"

Cube::Cube(cubeInfo info)
	: GameObject(info.position, info.rotation, info.name) {

	glm::vec3 color = info.color;
	vertices = {
		//Left Face
		{ { -0.5,-0.5, 0.5 },color,{ -1, 0, 0 } },
		{ { -0.5, 0.5, 0.5 },color,{ -1, 0, 0 } },
		{ { -0.5, 0.5,-0.5 },color,{ -1, 0, 0 } },

		{ { -0.5, 0.5,-0.5 },color,{ -1, 0, 0 } },
		{ { -0.5,-0.5,-0.5 },color,{ -1, 0, 0 } },
		{ { -0.5,-0.5, 0.5 },color,{ -1, 0, 0 } },

		//Right Face
		{ { 0.5, 0.5,-0.5 },color,{ 1, 0, 0 } },
		{ { 0.5, 0.5, 0.5 },color,{ 1, 0, 0 } },
		{ { 0.5,-0.5, 0.5 },color,{ 1, 0, 0 } },

		{ { 0.5,-0.5, 0.5 },color,{ 1, 0, 0 } },
		{ { 0.5,-0.5,-0.5 },color,{ 1, 0, 0 } },
		{ { 0.5, 0.5,-0.5 },color,{ 1, 0, 0 } },

		//Front face
		{ { -0.5,-0.5, 0.5 },color,{ 0, 0,  1 } },
		{ { 0.5,-0.5, 0.5 },color,{ 0, 0,  1 } },
		{ { 0.5, 0.5, 0.5 },color,{ 0, 0,  1 } },

		{ { 0.5, 0.5, 0.5 },color,{ 0, 0,  1 } },
		{ { -0.5, 0.5, 0.5 },color,{ 0, 0,  1 } },
		{ { -0.5,-0.5, 0.5 },color,{ 0, 0,  1 } },

		//Back Face
		{ { 0.5, 0.5,-0.5 },color,{ 0, 0, -1 } },
		{ { 0.5,-0.5,-0.5 },color,{ 0, 0, -1 } },
		{ { -0.5,-0.5,-0.5 },color,{ 0, 0, -1 } },

		{ { -0.5,-0.5,-0.5 },color,{ 0, 0, -1 } },
		{ { -0.5, 0.5,-0.5 },color,{ 0, 0, -1 } },
		{ { 0.5, 0.5,-0.5 },color,{ 0, 0, -1 } },

		//Bottom Face
		{ { 0.5, -0.5,-0.5 },color,{ 0, -1, 0 } },
		{ { -0.5, -0.5,-0.5 },color,{ 0, -1, 0 } },
		{ { -0.5, -0.5, 0.5 },color,{ 0, -1, 0 } },

		{ { -0.5, -0.5, 0.5 },color,{ 0,-1, 0 } },
		{ { 0.5, -0.5, 0.5 },color,{ 0,-1, 0 } },
		{ { 0.5, -0.5,-0.5 },color,{ 0,-1, 0 } },

		//Top Face
		{ { -0.5, 0.5, 0.5 },color,{ 0, 1, 0 } },
		{ { -0.5, 0.5,-0.5 },color,{ 0, 1, 0 } },
		{ { 0.5, 0.5,-0.5 },color,{ 0, 1, 0 } },

		{ { 0.5, 0.5,-0.5 },color,{ 0, 1, 0 } },
		{ { 0.5, 0.5, 0.5 },color,{ 0, 1, 0 } },
		{ { -0.5, 0.5, 0.5 },color,{ 0, 1, 0 } }
	};

	for (int v = 0; v < vertices.size(); v+=3) {
		vertices.at(v).scale(info.size.x, info.size.y, info.size.z);
		vertices.at(v+1).scale(info.size.x, info.size.y, info.size.z);
		vertices.at(v+2).scale(info.size.x, info.size.y, info.size.z);
	}

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	buffers.VAO = VAO;
	buffers.VBO = VBO;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	box = new BoundingBox(position, glm::vec3(info.size.x, info.size.y, info.size.z), \
		info.rotation, info.staticBox, info.friction, info.gravity);
}

Cube::~Cube()
{
	delete box;
}

void Cube::render()
{
	GLuint VAO = buffers.VAO;
	GLuint program = engine->getProgram();
	Camera* camera = engine->getCamera();

	glUseProgram(program);

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewPosLoc = glGetUniformLocation(program, "viewPos");

	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(viewPosLoc, camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Cube::update()
{
	GameObject::update();

	box->update(deltaTime);

	position = box->position;
	rotation = box->rotation;
}

bool Cube::createNew(luabridge::LuaRef table) {
	if (!table.isTable())
		return false;

	cubeInfo info;

	//Check to be sure that all of the needed values are present, or leave them as the default value
	luabridge::LuaRef position = table["position"];
	if (position.isTable() && position[1].isNumber() && position[2].isNumber() \
		&& position[3].isNumber())
		info.position = glm::vec3(position[1], position[2], position[3]);

	luabridge::LuaRef rotation = table["rotation"];
	if (rotation.isTable() && rotation[1].isNumber() && rotation[2].isNumber() \
		&& rotation[3].isNumber())
		info.rotation = glm::vec3(rotation[1], rotation[2], rotation[3]);

	luabridge::LuaRef color = table["color"];
	if (color.isTable() && color[1].isNumber() && color[2].isNumber() && color[3].isNumber())
		info.color = glm::vec3(color[1], color[2], color[3]);

	luabridge::LuaRef size = table["size"];
	if (size.isTable() && size[1].isNumber() && size[2].isNumber() && size[3].isNumber())
		info.size = glm::vec3(size[1], size[2], size[3]);

	if (table["staticBox"].isNumber())
		info.staticBox = table["staticBox"];

	if (table["friction"].isNumber())
		info.friction = table["friction"];

	if (table["gravity"].isNumber())
		info.gravity = table["gravity"];

	if (table["name"].isString()) 
		info.name = table["name"].cast<std::string>();

	//Creates the cube
	Cube* cube = new Cube(info);

	//Loads the object's lua script if it is given
	if (table["script"].isString()) {
		Engine::loadLuaScript(table["script"].cast<std::string>().c_str(), (GameObject*)cube);
		luabridge::setGlobal(cube->scriptState, engine, "Engine");
		luabridge::setGlobal(cube->scriptState, cube, "this");
	}
		
	//Adds the object to the engine's list of objects
	engine->addObject((GameObject*) cube);

	return true;
}

void Cube::moveWithCamera(float speed, bool lockY)
{
	GLfloat prevY = box->velocity.y;

	box->velocity += deltaTime * speed * cam->cameraFront;

	//Prevents the movement in the y direction if locked (prevents boosting jumps by looking up and moving)
	if (lockY)
		box->velocity.y = prevY;
}

void Cube::moveLeftByCamera(float speed)
{
	box->velocity -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * deltaTime * speed;
}

void Cube::setColor(float x, float y, float z)
{
	//Changes the vertices so that the color is correct
	glm::vec3 newColor = glm::vec3(x, y, z);
	for (int v = 0; v < vertices.size(); v++) {
		Vertex newV;
		newV.normal = vertices.at(v).normal;
		newV.pos = vertices.at(v).pos;
		newV.color = newColor;
		vertices[v] = newV;
	}

	//Copies the new list of vertices into video memory
	GLuint VAO = buffers.VAO;
	GLuint VBO = buffers.VBO;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Cube::addToLua(lua_State* luaState)
{
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("engine")
			.deriveClass <Cube, GameObject> ("Cube")
				.addStaticFunction("createNew", &Cube::createNew)
				.addFunction("moveWithCamera", &Cube::moveWithCamera)
				.addFunction("moveLeftByCamera", &Cube::moveLeftByCamera)
				.addFunction("setColor", &Cube::setColor)
			.endClass()
		.endNamespace();
}
