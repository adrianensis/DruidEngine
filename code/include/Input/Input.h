#ifndef DE_INPUT_H_
#define DE_INPUT_H_

#include "DE_Class.h"

#include "RenderContext.h"
#include "Vector2.h"

namespace DE {

class Input : public DE_Class{

private:

	static Vector2 smMouseCoordinates;
	static u32 smLastMouseButtonPressed;
	static u32 smLastKeyPressed;
	static bool keyJustPressed;
	static bool buttonJustPressed;
	static f32 scroll;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:

	DE_CLASS(Input, DE_Class);

	static void init();
	static void pollEvents();
	static bool isKeyPressedOnce(u32 key);
	static bool isKeyPressed(u32 key);
	static bool isMouseButtonPressedOnce(u32 key);
	static bool isMouseButtonPressed(u32 key);
	static Vector2 getMousePosition();

	static f32 getScroll();

	static void clearMouseButton();
	static void clearKey();
};

} /* namespace DE */

#endif /* DE_INPUT_H_ */
