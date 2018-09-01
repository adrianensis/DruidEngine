#ifndef DE_RENDERENGINE_H
#define DE_RENDERENGINE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace DE {

class RenderEngine {
private:
	GLFWwindow* mWindow;
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
	RenderEngine();
	virtual ~RenderEngine();

	void init();
	bool isClosed();
	void step();
	void terminate();
};

} /* namespace DE */

#endif /* DE_RENDERENGINE_H */
