#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIUtils.hpp"
#include "Maths/Geometry.hpp"

class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = SFun(void(UIElement *uiElement));

CLASS(FunctorUIElement, Functor<UIElementCallback>)
{
	PUB(UIElement *, UIElement, NONE)

public:

	virtual void execute() override
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
	PRO(bool, ReleaseOnSameGroupPressed, GET_SET)
	PRO(bool, Toggled, GET)
    PRO(bool, OnlyReleaseOnClickOutside, GET)

protected:

    void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseEvents();
	void subscribeToScrollEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	virtual void onChar(char character);
    void onBackspace();

    void onPressedEventReceived();
	void onReleasedEventReceived();

	virtual void onPrePressed() { };
	virtual void onPreReleased() { };
    virtual void onPostPressed() { };
	virtual void onPostReleased() { };

	virtual void onMouseOver();

	virtual void onScroll(f32 scroll);

	virtual void onFocusLost();
	void onFocus();

	void press();
	void executePressAndRelease(bool force = false);
	void release(bool force = false);

public:
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config);
	virtual void onDestroy();

	void simulateClick();

	bool hasFocus() const;

	bool isMouseCursorInsideElement();

	virtual void setText(const SStr &text) { };

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

	virtual void setVisibility(bool visibility);
	bool isVisible();
};