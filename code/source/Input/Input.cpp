#include "Input.h"
#include "Log.h"

namespace DE {

Vector2 Input::smMouseCoordinates = Vector2();
u32 Input::smLastMouseButtonPressed = 0;
u32 Input::smLastKeyPressed = 0;
bool Input::keyJustPressed = false;
bool Input::buttonJustPressed = false;

// ---------------------------------------------------------------------------

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
    	smLastKeyPressed = key;
			keyJustPressed = true;
		} else if (action == GLFW_RELEASE) {
			smLastKeyPressed = 0;
			keyJustPressed = false;
		}
}

// ---------------------------------------------------------------------------

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS){
			smLastMouseButtonPressed = button;
			buttonJustPressed = true;
		} else if (action == GLFW_RELEASE) {
			smLastMouseButtonPressed = 0;
			buttonJustPressed = false;
		}
}

// ---------------------------------------------------------------------------

Input::Input() : DE_Class() {

}

Input::~Input() {

}

// ---------------------------------------------------------------------------

void Input::init() {
	TRACE();

	glfwSetKeyCallback(RenderContext::smWindow, keyCallback);
	glfwSetMouseButtonCallback(RenderContext::smWindow, mouseButtonCallback);
}

// ---------------------------------------------------------------------------

void Input::pollEvents() {

  keyJustPressed = false;
  buttonJustPressed = false;

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

bool Input::isMouseButtonPressedOnce(u32 key) {
	return buttonJustPressed && key == smLastMouseButtonPressed;
}

// ---------------------------------------------------------------------------

bool Input::isMouseButtonPressed(u32 key) {
	return key == smLastMouseButtonPressed;
}

// ---------------------------------------------------------------------------

Vector2 Input::getMousePosition() {

	f64 mouseCoordX, mouseCoordY;

  glfwGetCursorPos(RenderContext::smWindow, &mouseCoordX, &mouseCoordY);

	f64 halfWindowSizeX = RenderContext::smWindowSize.x/2.0;
	f64 halfWindowSizeY = RenderContext::smWindowSize.y/2.0;

	mouseCoordX = mouseCoordX - halfWindowSizeX;
	mouseCoordY = halfWindowSizeY - mouseCoordY;

	smMouseCoordinates.set(mouseCoordX / halfWindowSizeX, mouseCoordY / halfWindowSizeY);

  return smMouseCoordinates;
}


// ---------------------------------------------------------------------------

} /* namespace DE */
