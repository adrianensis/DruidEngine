#ifndef DE_RENDERCONTEXT_H
#define DE_RENDERCONTEXT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "DE_Class.h"
#include "Array.h"

namespace DE {

class RenderContext : public DE_Class{
private:
	static GLFWwindow* mWindow;

	static void onResize(GLFWwindow* window, int width, int height);
public:
	RenderContext();
	~RenderContext() override;

	static void init();
	static bool isClosed();
	static void swap();
	static void terminate();

	static GLuint createVBO(const Array<f32>* data, u32 elementSize, u32 attributeArrayIndex);
	static GLuint createEBO(const Array<u32>* data);
	static GLuint createVAO();

	static void enableAttribute(const u32 attributeArrayIndex);
	static void disableAttribute(const u32 attributeArrayIndex);
	static void enableVAO(const u32 VAO);
};

} /* namespace DE */

#endif /* DE_RENDERCONTEXT_H */
