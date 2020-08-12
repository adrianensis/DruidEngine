#include "RenderContext.hpp"

//#include <gl/gl.h>
//#include <gl/glext.h>
#include <stddef.h>
#include <iostream>
#include <string>

#include "Array.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Camera.hpp"

namespace DE {

GLFWwindow* RenderContext::smWindow = nullptr;
Vector2 RenderContext::smWindowSize;

// ---------------------------------------------------------------------------

RenderContext::RenderContext() : DE_Class() {
};

RenderContext::~RenderContext() = default;

// ---------------------------------------------------------------------------

Vector2 RenderContext::getWindowSize() {
	return smWindowSize;
}

// ---------------------------------------------------------------------------

f32 RenderContext::getAspectRatio() {
	return smWindowSize.x / smWindowSize.y;
}

// ---------------------------------------------------------------------------

void RenderContext::onResize(GLFWwindow *window, int width, int height) {
	smWindowSize.set(width, height);
	glViewport(0, 0, smWindowSize.x, smWindowSize.y);
}

// ---------------------------------------------------------------------------

void RenderContext::init() {
	TRACE();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	smWindowSize.set(1080, 720);

	smWindow = glfwCreateWindow(smWindowSize.x, smWindowSize.y, "DruidEngine",
	NULL, NULL);
	if (!smWindow) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	} else {
		glfwMakeContextCurrent(smWindow);

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	glfwSetFramebufferSizeCallback(smWindow, RenderContext::onResize);

	//glViewport(0, 0, smWindowSize.x*10, smWindowSize.y*10);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	glEnable(GL_CULL_FACE); // BACK by default
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Input::init();

	RenderContext::clear();
}

// ---------------------------------------------------------------------------

bool RenderContext::isClosed() {
	return glfwWindowShouldClose(smWindow);
}

// ---------------------------------------------------------------------------

void RenderContext::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and the depth buffer
}

// ---------------------------------------------------------------------------

void RenderContext::swap() {
	glfwSwapBuffers(smWindow);
	RenderContext::clear();
}

// ---------------------------------------------------------------------------

void RenderContext::terminate() {
	glfwDestroyWindow(smWindow);
	glfwTerminate();
}

// ---------------------------------------------------------------------------

GLuint RenderContext::createVBO(const Array<f32> *data, u32 elementSize, u32 attributeArrayIndex) {

	DE_ASSERT(data != nullptr, "Data must be not null.");

	u32 VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);

	// for vertices elementSize should be 3 (x,y,z), for colors 4 (r,g,b,a)
	glVertexAttribPointer(attributeArrayIndex, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(f32), (byte*) 0);
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

GLuint RenderContext::createEBO(const Array<u32> *data) {

	DE_ASSERT(data != nullptr, "Data must be not null.");

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(),
	GL_STATIC_DRAW);

	return EBO;
}

// ---------------------------------------------------------------------------

void RenderContext::enableAttribute(u32 attributeArrayIndex) {
	glEnableVertexAttribArray(attributeArrayIndex);
}

// ---------------------------------------------------------------------------

void RenderContext::disableAttribute(u32 attributeArrayIndex) {
	glDisableVertexAttribArray(attributeArrayIndex);
}

// ---------------------------------------------------------------------------

void RenderContext::enableVAO(u32 VAO) {
	glBindVertexArray(VAO);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
