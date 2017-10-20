#include "GameObject.h"

void GameObject::addToLua(lua_State * luaState)
{
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("engine")
			.beginClass <GameObject>("GameObject")
				.addFunction("getPositionX", &GameObject::getPositionX)
				.addFunction("getPositionY", &GameObject::getPositionY)
				.addFunction("getPositionZ", &GameObject::getPositionZ)

				.addFunction("getRotationX", &GameObject::getRotationX)
				.addFunction("getRotationY", &GameObject::getRotationY)
				.addFunction("getRotationZ", &GameObject::getRotationZ)

				.addFunction("getVelocityX", &GameObject::getVelocityX)
				.addFunction("getVelocityY", &GameObject::getVelocityY)
				.addFunction("getVelocityZ", &GameObject::getVelocityZ)

				.addFunction("setPosition", &GameObject::setPosition)
				.addFunction("setRotation", &GameObject::setRotation)
				.addFunction("setVelocity", &GameObject::setVelocity)

				.addData("name", &GameObject::name)
			.endClass()
		.endNamespace();
}

void GameObject::update()
{
	//Calculates time since last loop
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	//Runs the lua script if it exists
	if (scriptState) {
		luabridge::LuaRef loop = luabridge::getGlobal(scriptState, "loop");
		if (loop.isFunction()) {
			luabridge::setGlobal(scriptState, deltaTime, "deltaTime");
			loop();
		}
	}
}

GameObject::GameObject(glm::vec3 _position, glm::vec3 _rotation, std::string _name)
{
	position = _position;
	rotation = _rotation;
	name = _name;

	scriptState = 0;
	lastTime = 0;
}
