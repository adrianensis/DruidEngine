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

class FunctorUIElement: public Functor<UIElementCallback>
{
	GENERATE_METADATA(FunctorUIElement)
	PUB_M(UIElement *, UIElement, NONE)

PUB

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

class UIElement: public GameObject
{
    GENERATE_METADATA(UIElement)
	PRO_M(UIElementConfig, Config, GETREF_CONST_SET)

	PRO_M(FunctorUIElement, OnPressedFunctor, NONE)
	PRO_M(FunctorUIElement, OnReleasedFunctor, NONE)

	PRO_M(FunctorUIElement, OnScrollFunctor, NONE)

	PRO_M(FunctorUIElement, OnTextChangedFunctor, NONE)
	PRO_M(FunctorUIElement, OnFocusLostFunctor, NONE)

	PRO_M(Renderer *, Renderer, GET)
	//PRI_M(Collider*, Collider, GET)
	PRO_M(SStr, InputString, GET)
	PRO_M(bool, ConsumeInput, GET_SET)
	PRO_M(bool, Pressed, GET)
	PRO_M(bool, CanToggle, GET)
	PRO_M(bool, ReleaseOnSameGroupPressed, GET_SET)
	PRO_M(bool, Toggled, GET)
    PRO_M(bool, OnlyReleaseOnClickOutside, GET)

PRO

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

PUB
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