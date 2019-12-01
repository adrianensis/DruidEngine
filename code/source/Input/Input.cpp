#include "Input.h"
#include "Log.h"

namespace DE {

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

	mouseCoordX = mouseCoordX - (RenderContext::smWindowSize.x/2.0f);
	mouseCoordY = (RenderContext::smWindowSize.y/2.0f) - mouseCoordY;

	mouseCoordX = mouseCoordX / (RenderContext::smWindowSize.x / 2.0f);
	mouseCoordY = mouseCoordY / (RenderContext::smWindowSize.y / 2.0f);

  return Vector2(mouseCoordX, mouseCoordY);
}


// ---------------------------------------------------------------------------

} /* namespace DE */
