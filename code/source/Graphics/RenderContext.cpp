#include "RenderContext.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <stddef.h>
#include <iostream>
#include <string>

namespace DE {

GLFWwindow* RenderContext::mWindow = nullptr;

RenderContext::RenderContext() : DE_Class()
{
};

RenderContext::~RenderContext() = default;

void RenderContext::onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RenderContext::init() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(800, 600, "DruidEngine", NULL, NULL);
	if (mWindow == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}else{
		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
		    std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	glfwSetFramebufferSizeCallback(mWindow, RenderContext::onResize);

	glViewport(0, 0, 800, 600);

	glClearColor(0.2f, 1.0f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	glEnable(GL_CULL_FACE); // BACK by default
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

bool RenderContext::isClosed() {
	return glfwWindowShouldClose(mWindow);
}

void RenderContext::swap() {
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void RenderContext::terminate() {
	glfwTerminate();
}

GLuint RenderContext::createVBO(const Array<f32>* data, const u32 elementSize, const u32 attributeArrayIndex) {
	if(data != nullptr){
		u32 VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);

		// for vertices elementSize should be 3 (x,y,z), for colors 4 (r,g,b,a)
		glVertexAttribPointer(attributeArrayIndex, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(f32), (void*)0);
		RenderContext::enableAttribute(attributeArrayIndex);

		return VBO;
	}else{
			return -1;
		}
}

GLuint RenderContext::createVAO() {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	RenderContext::enableVAO(VAO);

	return VAO;
}

GLuint RenderContext::createEBO(const Array<u32>* data) {
	if(data != nullptr){
		unsigned int EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);

		return EBO;
	}else{
		return -1;
	}
}

void RenderContext::enableAttribute(const u32 attributeArrayIndex){
	glEnableVertexAttribArray(attributeArrayIndex);
}

void RenderContext::disableAttribute(const u32 attributeArrayIndex){
	glDisableVertexAttribArray(attributeArrayIndex);
}

void RenderContext::enableVAO(const u32 VAO) {
	glBindVertexArray(VAO);
}

} /* namespace DE */
