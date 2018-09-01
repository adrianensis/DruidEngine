#include "RenderEngine.h"

#include <iostream>


namespace DE {

RenderEngine::RenderEngine() {
	// TODO Auto-generated constructor stub
}

RenderEngine::~RenderEngine() {
	// TODO Auto-generated destructor stub
}

void RenderEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RenderEngine::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(800, 600, "DruidEngine", NULL, NULL);
	if (mWindow == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}else{
		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
		    std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glViewport(0, 0, 800, 600);
}

bool RenderEngine::isClosed() {
	return glfwWindowShouldClose(mWindow);
}

void RenderEngine::step() {
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

} /* namespace DE */
