#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"

#include "Graphics/RenderContext.hpp"
#include "Maths/Vector2.hpp"

#include "Input/InputEvents.hpp"

class Input: public ObjectBase, public Singleton<Input>
{
	GENERATE_METADATA(Input)
PRI
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
	static void charCallback(GLFWwindow * window, unsigned int codepoint);

PUB
	Vector2 smMouseCoordinates;
	u32 smLastMouseButtonPressed;
	u32 smLastKeyPressed;
	u32 smModifier;
	bool smKeyJustPressed;
	bool smButtonJustPressed;
	f32 smScroll;

	void init();
	void pollEvents();
	bool isKeyPressedOnce(u32 key);
	bool isKeyPressed(u32 key);
	bool isModifierPressed(u32 modifier);
	bool isMouseButtonPressedOnce(u32 button);
	bool isMouseButtonPressed(u32 button);
	const Vector2& getMousePosition();

	f32 getScroll();

	void clearMouseButton();
	void clearKey();
};