#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera/Camera.hpp"

//#include <gl/gl.h>
//#include <gl/glext.h>
#include <stddef.h>

//#include "Input/Input.hpp"

GLFWwindow *RenderContext::smWindow = nullptr;
Vector2 RenderContext::smWindowSize;

Vector2 RenderContext::getWindowSize()
{
	return smWindowSize;
}

f32 RenderContext::getAspectRatio()
{
	return smWindowSize.x / smWindowSize.y;
}

void RenderContext::onResize(GLFWwindow *window, int width, int height)
{
	smWindowSize.set(width, height);
	glViewport(0, 0, smWindowSize.x, smWindowSize.y);
	RenderEngine::getInstance().getCamera()->onResize();
}

void RenderContext::init()
{
	TRACE()

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE); // https://stackoverflow.com/questions/50412575/is-there-a-way-to-remove-60-fps-cap-in-glfw

	smWindowSize.set(1080, 720);

	smWindow = glfwCreateWindow(smWindowSize.x, smWindowSize.y, "Engine", NULL, NULL);

	if (!smWindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	else
	{
		glfwMakeContextCurrent(smWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	glfwSetFramebufferSizeCallback(smWindow, RenderContext::onResize);

	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL);	 // Near things obscure far things
	glEnable(GL_CULL_FACE);	 // BACK by default
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	RenderContext::clear();
}

bool RenderContext::isClosed()
{
	return glfwWindowShouldClose(smWindow);
}

void RenderContext::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and the depth buffer
}

void RenderContext::swap()
{
	glFlush(); // https://stackoverflow.com/questions/50412575/is-there-a-way-to-remove-60-fps-cap-in-glfw
	//glfwSwapBuffers(smWindow);
	RenderContext::clear();
}

void RenderContext::terminate()
{
	glfwDestroyWindow(smWindow);
	glfwTerminate();
}

GLuint RenderContext::createVBO(u32 elementSize, u32 PropertyArrayIndex)
{
	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// for vertices elementSize should be 3 (x,y,z), for colors 4 (r,g,b,a)
	RenderContext::enableProperty(PropertyArrayIndex);
	glVertexAttribPointer(PropertyArrayIndex, elementSize, GL_FLOAT, GL_FALSE, elementSize * sizeof(f32), (byte *)0);
	return VBO;
}

GLuint RenderContext::createVAO()
{
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	RenderContext::enableVAO(VAO);
	return VAO;
}

GLuint RenderContext::createEBO()
{
	u32 EBO;
	glGenBuffers(1, &EBO);
	return EBO;
}

void RenderContext::resizeVBO(u32 VBO, u32 size, u32 drawMode /*= GL_DYNAMIC_DRAW*/)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * size, nullptr, drawMode);
}

void RenderContext::resizeEBO(u32 EBO, u32 size, u32 drawMode /*= GL_DYNAMIC_DRAW*/)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * size, nullptr, drawMode);
}

void RenderContext::setDataVBO(u32 VBO, const std::vector<f32> &data)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f32) * data.size(), data.data());
}

void RenderContext::setDataEBO(u32 EBO, const std::vector<u32> &data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(u32) * data.size(), data.data());
}

void RenderContext::enableProperty(u32 PropertyArrayIndex)
{
	glEnableVertexAttribArray(PropertyArrayIndex);
}

void RenderContext::disableProperty(u32 PropertyArrayIndex)
{
	glDisableVertexAttribArray(PropertyArrayIndex);
}

void RenderContext::enableVAO(u32 VAO)
{
	glBindVertexArray(VAO);
}

void RenderContext::drawRectangles(u32 rectanglesCount)
{
	glDrawElements(GL_TRIANGLES, rectanglesCount * 6, GL_UNSIGNED_INT, 0);
}

void RenderContext::drawLines(u32 linesCount)
{
	glDrawElements(GL_LINES, linesCount * 2, GL_UNSIGNED_INT, 0);
}