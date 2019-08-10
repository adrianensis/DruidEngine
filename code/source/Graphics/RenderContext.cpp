#include "RenderContext.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <stddef.h>
#include <iostream>
#include <string>

#include "Array.h"
#include "Camera.h"

namespace DE {

GLFWwindow* RenderContext::mWindow = nullptr;

// ---------------------------------------------------------------------------

RenderContext::RenderContext() : DE_Class()
{
};

RenderContext::~RenderContext() = default;

// ---------------------------------------------------------------------------

void RenderContext::onResize(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------

void RenderContext::init() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(800, 600, "DruidEngine", NULL, NULL);
	if (!mWindow){
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	//glEnable(GL_CULL_FACE); // BACK by default
	//glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	RenderContext::clear();
}

// ---------------------------------------------------------------------------

bool RenderContext::isClosed() {
	return glfwWindowShouldClose(mWindow);
}

// ---------------------------------------------------------------------------

void RenderContext::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Clear the color and the depth buffer
}

// ---------------------------------------------------------------------------

void RenderContext::swap() {
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	RenderContext::clear();
}

// ---------------------------------------------------------------------------

void RenderContext::terminate() {
	glfwTerminate();
}

// ---------------------------------------------------------------------------

GLuint RenderContext::createVBO(const Array<f32>* data, u32 elementSize, u32 attributeArrayIndex) {

  DE_ASSERT(data != nullptr, "Data must be not null.");

  u32 VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);

	// for vertices elementSize should be 3 (x,y,z), for colors 4 (r,g,b,a)
	glVertexAttribPointer(attributeArrayIndex, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(f32), (void*)0);
	RenderContext::enableAttribute(attributeArrayIndex);

  return VBO;
}

// ---------------------------------------------------------------------------

GLuint RenderContext::createVAO() {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	RenderContext::enableVAO(VAO);

	return VAO;
}

// ---------------------------------------------------------------------------

GLuint RenderContext::createEBO(const Array<u32>* data) {

  DE_ASSERT(data != nullptr, "Data must be not null.");

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);

	return EBO;
}

// ---------------------------------------------------------------------------

void RenderContext::enableAttribute(u32 attributeArrayIndex){
	glEnableVertexAttribArray(attributeArrayIndex);
}

// ---------------------------------------------------------------------------

void RenderContext::disableAttribute(u32 attributeArrayIndex){
	glDisableVertexAttribArray(attributeArrayIndex);
}

// ---------------------------------------------------------------------------

void RenderContext::enableVAO(u32 VAO) {
	glBindVertexArray(VAO);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
