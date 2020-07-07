#ifndef DE_INPUT_H_
#define DE_INPUT_H_

#include "DE_Class.hpp"

#include "RenderContext.hpp"
#include "Vector2.hpp"

namespace DE {

class Input: public DE_Class {

private:

	static Vector2 smMouseCoordinates;
	static u32 smLastMouseButtonPressed;
	static u32 smLastKeyPressed;
	static u32 smModifier;
	static bool keyJustPressed;
	static bool buttonJustPressed;
	static f32 scroll;

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

public:

	DE_CLASS(Input, DE_Class)
	;

	static void init();
	static void pollEvents();
	static bool isKeyPressedOnce(u32 key);
	static bool isKeyPressed(u32 key);
	static bool isModifierPressed(u32 modifier);
	static bool isMouseButtonPressedOnce(u32 button);
	static bool isMouseButtonPressed(u32 button);
	static Vector2 getMousePosition();

	static f32 getScroll();

	static void clearMouseButton();
	static void clearKey();
};

} /* namespace DE */

#endif /* DE_INPUT_H_ */
