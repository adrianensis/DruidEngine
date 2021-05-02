#include "Input/Input.hpp"
#include "Log/Log.hpp"
#include "Events/EventsManager.hpp"

namespace DE {

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	Input::getInstance()->smModifier = mods;

	if (action == GLFW_PRESS) {
		Input::getInstance()->smLastKeyPressed = key;
		Input::getInstance()->smKeyJustPressed = true;

		if(key == GLFW_KEY_ENTER){
			InputEventKeyEnter event;
			SEND_INPUT_EVENT(event);
		} else if(key == GLFW_KEY_ESCAPE) {
			InputEventKeyEsc event;
			SEND_INPUT_EVENT(event);
		} else {
			InputEventKeyPressed event;
			event.mKey = key;
			event.mMods = mods;
			SEND_INPUT_EVENT(event);
		}

	} else if (action == GLFW_RELEASE) {

		InputEventKeyReleased event;
		event.mKey = key;
		event.mMods = mods;
		SEND_INPUT_EVENT(event);

		Input::getInstance()->clearKey();
	}
}

void Input::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	Input::getInstance()->smModifier = mods;

	if (action == GLFW_PRESS) {

		Input::getInstance()->smLastMouseButtonPressed = button;
		Input::getInstance()->smButtonJustPressed = true;

		InputEventMouseButtonPressed event;
		event.mButton = button;
		event.mMods = mods;
		SEND_INPUT_EVENT(event);
	} else if (action == GLFW_RELEASE) {

		InputEventMouseButtonReleased event;
		event.mButton = button;
		event.mMods = mods;
		SEND_INPUT_EVENT(event);

		Input::getInstance()->clearMouseButton();
	}
}

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	Input::getInstance()->smScroll = yoffset;

	InputEventScroll event;
	event.mScroll = yoffset;
	SEND_INPUT_EVENT(event);
}

void Input::charCallback(GLFWwindow* window, unsigned int codepoint) {
	InputEventChar event;
	event.mChar = (c8) codepoint;
	SEND_INPUT_EVENT(event);
}

Input::Input() : ObjectBase(), Singleton<Input>() {

}

Input::~Input() {

}

void Input::init() {
	TRACE()

	smMouseCoordinates = Vector2();
	smLastMouseButtonPressed = -1;
	smLastKeyPressed = -1;
	smModifier = -1;
	smKeyJustPressed = false;
	smButtonJustPressed = false;
	smScroll = 0;

	glfwSetKeyCallback(RenderContext::smWindow, keyCallback);
	glfwSetMouseButtonCallback(RenderContext::smWindow, mouseButtonCallback);
	glfwSetScrollCallback(RenderContext::smWindow, scrollCallback);
	glfwSetCharCallback(RenderContext::smWindow, charCallback);
}

void Input::pollEvents() {

	smKeyJustPressed = false;
	smButtonJustPressed = false;
	smScroll = 0;

	f64 mouseCoordX, mouseCoordY;

	glfwGetCursorPos(RenderContext::smWindow, &mouseCoordX, &mouseCoordY);

	f64 halfWindowSizeX = RenderContext::smWindowSize.x / 2.0;
	f64 halfWindowSizeY = RenderContext::smWindowSize.y / 2.0;

	mouseCoordX = mouseCoordX - halfWindowSizeX;
	mouseCoordY = halfWindowSizeY - mouseCoordY;

	bool moved = false;
	Vector2 newMouseCoordinates(mouseCoordX / halfWindowSizeX, mouseCoordY / halfWindowSizeY);

	if(!smMouseCoordinates.eq(newMouseCoordinates))
	{
		smMouseCoordinates.set(newMouseCoordinates);

		InputEventMouseMoved event;
		SEND_INPUT_EVENT(event);
	}

	glfwPollEvents();
}

bool Input::isKeyPressedOnce(u32 key) {
	return smKeyJustPressed && key == smLastKeyPressed;
}

bool Input::isKeyPressed(u32 key) {
	return key == smLastKeyPressed;
}

bool Input::isModifierPressed(u32 modifier) {
	return modifier == smModifier;
}

bool Input::isMouseButtonPressedOnce(u32 button) {
	return smButtonJustPressed && button == smLastMouseButtonPressed;
}

bool Input::isMouseButtonPressed(u32 button) {
	return button == smLastMouseButtonPressed;
}

Vector2 Input::getMousePosition() {

	return smMouseCoordinates;
}

f32 Input::getScroll() {
	return smScroll;
}

void Input::clearMouseButton() {
	smLastMouseButtonPressed = -1;
	smButtonJustPressed = false;
}
void Input::clearKey() {
	smLastKeyPressed = -1;
	smKeyJustPressed = false;
}

}
