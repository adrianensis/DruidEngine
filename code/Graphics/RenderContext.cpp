#include "Graphics/RenderContext.hpp"

//#include <gl/gl.h>
//#include <gl/glext.h>
#include <stddef.h>
#include <iostream>
#include <string>

#include "Containers/Array.hpp"
#include "Graphics/Camera.hpp"
#include "Input/Input.hpp"
#include "Graphics/Camera.hpp"

namespace DE {

GLFWwindow* RenderContext::smWindow = nullptr;
Vector2 RenderContext::smWindowSize;

RenderContext::RenderContext() : DE_Class() {
};

RenderContext::~RenderContext() = default;

Vector2 RenderContext::getWindowSize() {
	return smWindowSize;
}

f32 RenderContext::getAspectRatio() {
	return smWindowSize.x / smWindowSize.y;
}

void RenderContext::onResize(GLFWwindow *window, int width, int height) {
	smWindowSize.set(width, height);
	glViewport(0, 0, smWindowSize.x, smWindowSize.y);
}

void RenderContext::init() {
	DE_TRACE()

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE ); // https://stackoverflow.com/questions/50412575/is-there-a-way-to-remove-60-fps-cap-in-glfw

	smWindowSize.set(1080, 720);

	smWindow = glfwCreateWindow(smWindowSize.x, smWindowSize.y, "DruidEngine", NULL, NULL);

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

	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	glEnable(GL_CULL_FACE); // BACK by default
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Input::getInstance()->init();

	RenderContext::clear();
}

bool RenderContext::isClosed() {
	return glfwWindowShouldClose(smWindow);
}

void RenderContext::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and the depth buffer
}

void RenderContext::swap() {
	glFlush(); // https://stackoverflow.com/questions/50412575/is-there-a-way-to-remove-60-fps-cap-in-glfw
	//glfwSwapBuffers(smWindow);
	RenderContext::clear();
}

void RenderContext::terminate() {
	glfwDestroyWindow(smWindow);
	glfwTerminate();
}

GLuint RenderContext::createVBO(u32 elementSize, u32 PropertyArrayIndex) {
	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 1000, nullptr, GL_DYNAMIC_DRAW);
	// for vertices elementSize should be 3 (x,y,z), for colors 4 (r,g,b,a)
	RenderContext::enableProperty(PropertyArrayIndex);
	glVertexAttribPointer(PropertyArrayIndex, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(f32), (byte*) 0);
	return VBO;
}

GLuint RenderContext::createVAO() {
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	RenderContext::enableVAO(VAO);
	return VAO;
}

GLuint RenderContext::createEBO() {
	u32 EBO;
	glGenBuffers(1, &EBO);
	return EBO;
}

void RenderContext::setDataVBO(u32 VBO, const Array<f32> *data) {
	DE_ASSERT(data != nullptr, "Data must be not null.");
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_DYNAMIC_DRAW);
}

void RenderContext::setDataEBO(u32 EBO, const Array<u32> *data) {
	DE_ASSERT(data != nullptr, "Data must be not null.");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getElementSize() * data->getLength(), data->getRawData(), GL_STATIC_DRAW);
}

void RenderContext::enableProperty(u32 PropertyArrayIndex) {
	glEnableVertexAttribArray(PropertyArrayIndex);
}

void RenderContext::disableProperty(u32 PropertyArrayIndex) {
	glDisableVertexAttribArray(PropertyArrayIndex);
}

void RenderContext::enableVAO(u32 VAO) {
	glBindVertexArray(VAO);
}

void RenderContext::drawRectangles(u32 rectanglesCount) {
	glDrawElements(GL_TRIANGLES, rectanglesCount * 6, GL_UNSIGNED_INT, 0);
}

void RenderContext::drawLines(u32 linesCount) {
	glDrawElements(GL_LINES, linesCount * 2, GL_UNSIGNED_INT, 0);
}

}
