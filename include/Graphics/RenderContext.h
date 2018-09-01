#ifndef DE_RENDERCONTEXT_H
#define DE_RENDERCONTEXT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Array.h"

namespace DE {

class RenderContext {
private:
	static GLFWwindow* mWindow;

	static void onResize(GLFWwindow* window, int width, int height);
public:
	RenderContext();
	virtual ~RenderContext();

	static void init();
	static bool isClosed();
	static void swap();
	static void terminate();

	static GLuint createVBO(Array<f32>* data, u32 elementSize, u32 attributeArrayIndex);
	static GLuint createEBO(Array<u32>* data);
	static GLuint createVAO();

	static void enableAttribute(u32 attributeArrayIndex);
	static void disableAttribute(u32 attributeArrayIndex);
	static void enableVAO(u32 VAO);
};

} /* namespace DE */

#endif /* DE_RENDERCONTEXT_H */
