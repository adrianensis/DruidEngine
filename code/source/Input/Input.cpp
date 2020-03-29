#include "Input.h"
#include "Log.h"

namespace DE {

Vector2 Input::smMouseCoordinates = Vector2();
u32 Input::smMouseButton = 0;

// ---------------------------------------------------------------------------

Input::Input() : DE_Class() {

}

Input::~Input() {

}

// ---------------------------------------------------------------------------

void Input::init() {
	TRACE();
}

// ---------------------------------------------------------------------------

void Input::pollEvents() {
  glfwPollEvents();
}

// ---------------------------------------------------------------------------

bool Input::isKeyPressed(u32 key) {
  return glfwGetKey(RenderContext::smWindow, key) == GLFW_PRESS;
}

// ---------------------------------------------------------------------------

bool Input::isMouseButtonPressed(u32 key) {
  return glfwGetMouseButton(RenderContext::smWindow, key) == GLFW_PRESS;
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
