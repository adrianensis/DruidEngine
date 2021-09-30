#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"


class Camera;

class RenderContext
{
private:
	STC GLFWwindow *smWindow;

	STC Vector2 smWindowSize;

	STC Camera *mCamera;

	STC void onResize(GLFWwindow *window, int width, int height);

public:
	friend class Input;

	STC Vector2 getWindowSize();
	STC f32 getAspectRatio();

	STC void init();
	STC bool isClosed();
	STC void swap();
	STC void clear();
	STC void terminate();

	STC GLuint createVBO(u32 elementSize, u32 PropertyArrayIndex);
	STC GLuint createEBO();
	STC GLuint createVAO();
	STC void setDataVBO(u32 VBO, CNS SVec(f32) &data);
	STC void setDataEBO(u32 EBO, CNS SVec(u32) &data);

	STC void enableProperty(u32 propertyArrayIndex);
	STC void disableProperty(u32 propertyArrayIndex);
	STC void enableVAO(u32 VAO);

	STC void drawRectangles(u32 rectanglesCount);
	STC void drawLines(u32 linesCount);
};