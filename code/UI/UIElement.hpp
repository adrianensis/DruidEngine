#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIUtils.hpp"

class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = SFun(void(UIElement *uiElement));

CLASS(FunctorUIElement, Functor<UIElementCallback>)
{
	PUB(UIElement *, UIElement, NONE)

public:

	VIR void execute() OVR
	{
		if (mCallback)
		{
			mCallback(mUIElement);
		}
	}

	COPY(FunctorUIElement)
	{
		DO_COPY(UIElement)
		Functor<UIElementCallback>::copy(other);
	}
};

CLASS(UIElement, GameObject)
{
	PRO(UIElementConfig, Config, GETREF_CONST_SET)

	PRO(FunctorUIElement, OnPressedFunctor, NONE)
	PRO(FunctorUIElement, OnReleasedFunctor, NONE)

	PRO(FunctorUIElement, OnScrollFunctor, NONE)

	PRO(FunctorUIElement, OnTextChangedFunctor, NONE)
	PRO(FunctorUIElement, OnFocusLostFunctor, NONE)

	PRO(Renderer *, Renderer, GET)
	//PRI(Collider*, Collider, GET)
	PRO(SStr, InputString, GET)
	PRO(bool, ConsumeInput, GET_SET)
	PRO(bool, Pressed, GET)
	PRO(bool, CanToggle, GET)
	PRO(bool, ReleaseOnSameGroupPressed, GET)
	PRO(bool, Toggled, GET)

	void press();
	void executePressAndRelease(bool force = false);
	void release(bool force = false);

public:
	VIR void init() OVR;
	VIR void initFromConfig(const UIElementConfig& config);
	VIR void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseEvents();
	void subscribeToScrollEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	bool isMouseCursorInsideElement();

	VIR void onChar(char character);

	VIR void setText(const SStr &text) { };

	VIR void onPressed();
	VIR void onReleased();

	void simulateClick();

	VIR void onMouseOver();

	VIR void onScroll(f32 scroll);

	VIR void onFocusLost();
	void onFocus();

	void setOnPressedCallback(UIElementCallback callback)
	{
		mOnPressedFunctor.mUIElement = this;
		mOnPressedFunctor.setCallback(callback);
	}

	void setOnReleasedCallback(UIElementCallback callback)
	{
		mOnReleasedFunctor.mUIElement = this;
		mOnReleasedFunctor.setCallback(callback);
	}

	void setOnTextChangedCallback(UIElementCallback callback)
	{
		mOnTextChangedFunctor.mUIElement = this;
		mOnTextChangedFunctor.setCallback(callback);
	}

	void setOnFocusLostCallback(UIElementCallback callback)
	{
		mOnFocusLostFunctor.mUIElement = this;
		mOnFocusLostFunctor.setCallback(callback);
	}

	void setComponentsCache();

	VIR void setVisibility(bool visibility);
	bool isVisible();
};