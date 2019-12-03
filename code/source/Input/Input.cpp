#include "Input.h"
#include "Log.h"

namespace DE {

Vector2 Input::smMouseCoordinates = Vector2();
Vector2 Input::smHalfWindowSize = Vector2();
u32 Input::smMouseButton = 0;

// ---------------------------------------------------------------------------

Input::Input() : DE_Class() {

}

Input::~Input() {

}

// ---------------------------------------------------------------------------

void Input::init() {
	TRACE();

	smHalfWindowSize.set((RenderContext::smWindowSize.x/2.0f), (RenderContext::smWindowSize.y/2.0f));
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

	mouseCoordX = mouseCoordX - smHalfWindowSize.x;
	mouseCoordY = smHalfWindowSize.y - mouseCoordY;

	smMouseCoordinates.set(mouseCoordX / smHalfWindowSize.x, mouseCoordY / smHalfWindowSize.y);

  return smMouseCoordinates;
}


// ---------------------------------------------------------------------------

} /* namespace DE */
