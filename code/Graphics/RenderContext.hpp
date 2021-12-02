#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Core/Core.hpp"
#include "Maths/Maths.hpp"

class Camera;

class RenderContext
{
PRI
	static GLFWwindow *smWindow;

	static Vector2 smWindowSize;

	static Camera *mCamera;

	static void onResize(GLFWwindow *window, int width, int height);

PUB
	friend class Input;

	static Vector2 getWindowSize();
	static f32 getAspectRatio();

	static void init();
	static bool isClosed();
	static void swap();
	static void clear();
	static void terminate();

	static GLuint createVBO(u32 elementSize, u32 PropertyArrayIndex);
	static GLuint createEBO();
	static GLuint createVAO();
	static void resizeVBO(u32 VBO, u32 size, u32 drawMode = GL_DYNAMIC_DRAW);
	static void resizeEBO(u32 EBO, u32 size, u32 drawMode = GL_DYNAMIC_DRAW);
	static void setDataVBO(u32 VBO, const std::vector<f32> &data);
	static void setDataEBO(u32 EBO, const std::vector<u16> &data);

	static void enableProperty(u32 propertyArrayIndex);
	static void disableProperty(u32 propertyArrayIndex);
	static void enableVAO(u32 VAO);

	static void drawElements(u32 indicesCount, u32 instancesCount, bool instanced);
	static void drawLines(u32 linesCount);
};