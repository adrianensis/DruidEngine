#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include <string>

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
		Functor<UIElementCallback>::operator=(*other);
	}
};

CLASS(UIElement, GameObject)
{
	PRO(OnPressedFunctor, NONE, FunctorUIElement)
	PRO(OnReleasedFunctor, NONE, FunctorUIElement)

	PRO(OnTextChangedFunctor, NONE, FunctorUIElement)
	PRO(OnFocusLostFunctor, NONE, FunctorUIElement)

	PRI(Renderer, GET, Renderer *)
	//PRI(Collider, GET, Collider*)
	PRO(InputString, GET, std::string)
	PRO(ConsumeInput, GET_SET, bool)
	PRI(Group, GET_SET, UIGroup *)
	PRO(Pressed, GET, bool)

private:
	bool isMouseCursorInsideElement();

public:
	virtual void init() override;
	virtual void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	void onChar(char character);

	virtual void setText(const std::string &text) = 0;

	void onPressed();
	void onReleased();

	void onMouseOver();

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

	virtual void toggle(){};
};