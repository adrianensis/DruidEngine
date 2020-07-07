#include "Input.hpp"
#include "Log.hpp"

namespace DE {

Vector2 Input::smMouseCoordinates = Vector2();
u32 Input::smLastMouseButtonPressed = -1;
u32 Input::smLastKeyPressed = -1;
u32 Input::smModifier = -1;
bool Input::keyJustPressed = false;
bool Input::buttonJustPressed = false;
f32 Input::scroll = 0;

// ---------------------------------------------------------------------------

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		smLastKeyPressed = key;
		keyJustPressed = true;
	} else if (action == GLFW_RELEASE) {
		Input::clearKey();
	}

	smModifier = mods;
}

// ---------------------------------------------------------------------------

void Input::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		smLastMouseButtonPressed = button;
		buttonJustPressed = true;
	} else if (action == GLFW_RELEASE) {
		Input::clearMouseButton();
	}

	smModifier = mods;
}

// ---------------------------------------------------------------------------

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	scroll = yoffset;
}

// ---------------------------------------------------------------------------

Input::Input() :
		DE_Class() {

}

Input::~Input() {

}

// ---------------------------------------------------------------------------

void Input::init() {
	TRACE();

	glfwSetKeyCallback(RenderContext::smWindow, keyCallback);
	glfwSetMouseButtonCallback(RenderContext::smWindow, mouseButtonCallback);
	glfwSetScrollCallback(RenderContext::smWindow, scrollCallback);
}

// ---------------------------------------------------------------------------

void Input::pollEvents() {

	keyJustPressed = false;
	buttonJustPressed = false;
	scroll = 0;

	glfwPollEvents();
}

// ---------------------------------------------------------------------------

bool Input::isKeyPressedOnce(u32 key) {
	return keyJustPressed && key == smLastKeyPressed;
}

// ---------------------------------------------------------------------------

bool Input::isKeyPressed(u32 key) {
	return key == smLastKeyPressed;
}

// ---------------------------------------------------------------------------

bool Input::isModifierPressed(u32 modifier) {
	return modifier == smModifier;
}

// ---------------------------------------------------------------------------

bool Input::isMouseButtonPressedOnce(u32 button) {
	return buttonJustPressed && button == smLastMouseButtonPressed;
}

// ---------------------------------------------------------------------------

bool Input::isMouseButtonPressed(u32 button) {
	return button == smLastMouseButtonPressed;
}

// ---------------------------------------------------------------------------

Vector2 Input::getMousePosition() {

	f64 mouseCoordX, mouseCoordY;

	glfwGetCursorPos(RenderContext::smWindow, &mouseCoordX, &mouseCoordY);

	f64 halfWindowSizeX = RenderContext::smWindowSize.x / 2.0;
	f64 halfWindowSizeY = RenderContext::smWindowSize.y / 2.0;

	mouseCoordX = mouseCoordX - halfWindowSizeX;
	mouseCoordY = halfWindowSizeY - mouseCoordY;

	smMouseCoordinates.set(mouseCoordX / halfWindowSizeX, mouseCoordY / halfWindowSizeY);

	return smMouseCoordinates;
}

// ---------------------------------------------------------------------------

f32 Input::getScroll() {
	return scroll;
}

// ---------------------------------------------------------------------------

void Input::clearMouseButton() {
	smLastMouseButtonPressed = -1;
	buttonJustPressed = false;
}
void Input::clearKey() {
	smLastKeyPressed = -1;
	keyJustPressed = false;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
