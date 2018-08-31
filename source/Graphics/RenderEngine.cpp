#include "RenderEngine.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace DE {

RenderEngine::RenderEngine() {
	// TODO Auto-generated constructor stub
}

RenderEngine::~RenderEngine() {
	// TODO Auto-generated destructor stub
}

void RenderEngine::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

} /* namespace DE */
