#include "PrimEngine.h"

int WinMain() {
	Engine engine;

	engine.init("Prim Engine", 1290, 720, "shaders/default.vert", "shaders/default.frag", glm::vec3(0.529f, 0.808f, 0.981f));

	engine.run();
}