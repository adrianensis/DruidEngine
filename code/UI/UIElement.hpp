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

using UIElementCallback = std::function<void(UIElement *uiElement)>;

class FunctorUIElement: public Functor<UIElementCallback>
{
	GENERATE_METADATA(FunctorUIElement)
	PUB UIElement * mUIElement = {};

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
	PRO UIElementConfig mConfig = {}; GETREF_CONST_SET(Config)

	PRO FunctorUIElement mOnPressedFunctor = {};
	PRO FunctorUIElement mOnReleasedFunctor = {};

	PRO FunctorUIElement mOnScrollFunctor = {};

	PRO FunctorUIElement mOnTextChangedFunctor = {};
	PRO FunctorUIElement mOnFocusLostFunctor = {};

	PRO Renderer * mRenderer = {}; GET(Renderer)
	//PRI Collider* mCollider = {}; GET(Collider)
	PRO std::string mInputString = {}; GET(InputString)
	PRO bool mConsumeInput = {}; GET_SET(ConsumeInput)
	PRO bool mPressed = {}; GET(Pressed)
	PRO bool mCanToggle = {}; GET(CanToggle)
	PRO bool mReleaseOnSameGroupPressed = {}; GET_SET(ReleaseOnSameGroupPressed)
	PRO bool mToggled = {}; GET(Toggled)
    PRO bool mOnlyReleaseOnClickOutside = {}; GET(OnlyReleaseOnClickOutside)

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

	virtual void setText(const std::string &text) { };

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