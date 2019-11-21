#include "Input.h"

namespace DE {

// ---------------------------------------------------------------------------

Input::Input() : DE_Class() {

}

Input::~Input() {

}

// ---------------------------------------------------------------------------

void Input::init() {

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

Vector2 Input::getMousePosition() {
  f64 xPos, yPos;
  glfwGetCursorPos(RenderContext::smWindow, &xPos, &yPos);

  return Vector2(static_cast<f32>(xPos) - (RenderContext::smWindowSize.x/2.0f), (RenderContext::smWindowSize.y/2.0f) - static_cast<f32>(yPos));
}


// ---------------------------------------------------------------------------

} /* namespace DE */
