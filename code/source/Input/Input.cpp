#include "Input.hpp"
#include "Log.hpp"
#include "UIElement.hpp"

namespace DE {

Vector2 Input::smMouseCoordinates = Vector2();
u32 Input::smLastMouseButtonPressed = -1;
u32 Input::smLastKeyPressed = -1;
u32 Input::smModifier = -1;
bool Input::smKeyJustPressed = false;
bool Input::smButtonJustPressed = false;
f32 Input::smScroll = 0;
UIElement* Input::smUIElement = nullptr;

// ---------------------------------------------------------------------------

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		smLastKeyPressed = key;
		smKeyJustPressed = true;

		if(key == GLFW_KEY_ENTER && smUIElement){
			smUIElement->inputCloseCallback();
			smUIElement = nullptr;
		}

	} else if (action == GLFW_RELEASE) {
		Input::clearKey();
	}

	smModifier = mods;
}

// ---------------------------------------------------------------------------

void Input::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		smLastMouseButtonPressed = button;
		smButtonJustPressed = true;
	} else if (action == GLFW_RELEASE) {
		Input::clearMouseButton();
	}

	smModifier = mods;
}

// ---------------------------------------------------------------------------

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	smScroll = yoffset;
}

// ---------------------------------------------------------------------------

void Input::charCallback(GLFWwindow* window, unsigned int codepoint) {
	if(smUIElement){
		smUIElement->inputCharCallback((c8) codepoint);
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
	glfwSetScrollCallback(RenderContext::smWindow, scrollCallback);
	glfwSetCharCallback(RenderContext::smWindow, charCallback);
}

// ---------------------------------------------------------------------------

void Input::pollEvents() {

	smKeyJustPressed = false;
	smButtonJustPressed = false;
	smScroll = 0;

	glfwPollEvents();
}

// ---------------------------------------------------------------------------

bool Input::isKeyPressedOnce(u32 key) {
	return smKeyJustPressed && key == smLastKeyPressed;
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
	return smButtonJustPressed && button == smLastMouseButtonPressed;
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
	return smScroll;
}

// ---------------------------------------------------------------------------

void Input::clearMouseButton() {
	smLastMouseButtonPressed = -1;
	smButtonJustPressed = false;
}
void Input::clearKey() {
	smLastKeyPressed = -1;
	smKeyJustPressed = false;
}

// ---------------------------------------------------------------------------

void Input::setInputCharReceiver(UIElement* uiElement) {
	if(smUIElement){
		smUIElement->inputCloseCallback();
	}

	smUIElement = uiElement;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
