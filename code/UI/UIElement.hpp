#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIUtils.hpp"

class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = std::function<void(UIElement *uiElement)>;

CLASS(FunctorUIElement, Functor<UIElementCallback>)
{
	PUB(UIElement, NONE, UIElement *)

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
	PRO(Config, GETREF_CONST_SET, UIElementConfig)

	PRO(OnPressedFunctor, NONE, FunctorUIElement)
	PRO(OnReleasedFunctor, NONE, FunctorUIElement)

	PRO(OnScrollFunctor, NONE, FunctorUIElement)

	PRO(OnTextChangedFunctor, NONE, FunctorUIElement)
	PRO(OnFocusLostFunctor, NONE, FunctorUIElement)

	PRO(Renderer, GET, Renderer *)
	//PRI(Collider, GET, Collider*)
	PRO(InputString, GET, std::string)
	PRO(ConsumeInput, GET_SET, bool)
	PRO(Pressed, GET, bool)
	PRO(CanToggle, GET, bool)
	PRO(ReleaseOnSameGroupPressed, GET, bool)
	PRO(Toggled, GET, bool)

	void press();
	void executePressAndRelease(bool force = false);
	void release(bool force = false);

public:
	virtual void init() override;
	virtual void initFromConfig(const UIElementConfig& config);
	virtual void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseEvents();
	void subscribeToScrollEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	bool isMouseCursorInsideElement();

	virtual void onChar(char character);

	virtual void setText(const std::string &text) { };

	virtual void onPressed();
	virtual void onReleased();

	void simulateClick();

	virtual void onMouseOver();

	virtual void onScroll(f32 scroll);

	virtual void onFocusLost();
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

	virtual void setVisibility(bool visibility);
	bool isVisible();
};