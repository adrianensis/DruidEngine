#ifndef DE_RENDERCONTEXT_H
#define DE_RENDERCONTEXT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "DE_Class.hpp"
#include "Vector2.hpp"

namespace DE {

template<class T> class Array;
class Camera;

class RenderContext: public DE_Class {

private:

	static GLFWwindow* smWindow;

	static Vector2 smWindowSize;

	static Camera* mCamera;

	static void onResize(GLFWwindow *window, int width, int height);

public:

	friend class Input;

	RenderContext();
	~RenderContext() override;

	static Vector2 getWindowSize();
	static f32 getAspectRatio();

	static void init();
	static bool isClosed();
	static void swap();
	static void clear();
	static void terminate();

	static GLuint createVBO(u32 elementSize, u32 attributeArrayIndex);
	static GLuint createEBO();
	static GLuint createVAO();
	static void setDataVBO(u32 VBO, const Array<f32> *data);
	static void setDataEBO(u32 EBO, const Array<u32> *data);

	static void enableAttribute(u32 attributeArrayIndex);
	static void disableAttribute(u32 attributeArrayIndex);
	static void enableVAO(u32 VAO);

	static void drawTriangles(u32 indices);
};

} /* namespace DE */

#endif /* DE_RENDERCONTEXT_H */