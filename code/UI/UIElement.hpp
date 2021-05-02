#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include <string>

namespace DE {

class Collider;
class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = std::function<void(UIElement* uiElement)>;

class FunctorUIElement : public Functor<UIElementCallback> {
public:
	GENERATE_METADATA(FunctorUIElement);

	 UIElement* mUIElement;

	FunctorUIElement():Functor<UIElementCallback>() {
		mUIElement = nullptr;
	};
	~FunctorUIElement() override {};

	virtual void execute() override {
		if (mCallback)
			mCallback(mUIElement);
	}

	FunctorUIElement& operator= (const FunctorUIElement &functor) {
		// self-assignment guard
		if (this == &functor)
			return *this;

		// do the copy
		mUIElement = functor.mUIElement;
		Functor<UIElementCallback>::operator =(functor);

		return *this;
	}

};

class UIElement : public GameObject {

protected:

	 FunctorUIElement mOnPressedFunctor;
	 FunctorUIElement mOnReleasedFunctor;

	 FunctorUIElement mOnTextChangedFunctor;
	 FunctorUIElement mOnFocusLostFunctor;

	 Renderer* mRenderer;
	 Collider* mCollider;
	 String mInputString;
	 bool mConsumeInput;
	 UIGroup* mGroup;
	 bool mPressed;

public:

	GENERATE_METADATA(UIElement);

	UIElement();
	virtual ~UIElement() override;;

	GET(Renderer);
	GET(Collider);
	GET(InputString);
	GET_SET(ConsumeInput);
	GET_SET(Group);
	GET(Pressed);

	virtual void init() override;
	virtual void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseButtonEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	void onChar(c8 character);

	virtual void setText(const String &text) = 0;

	void onPressed();
	void onReleased();

	virtual void onFocusLost();
	void onFocus();

	void setOnPressedCallback(UIElementCallback callback) {
		mOnPressedFunctor.mUIElement = this;
		mOnPressedFunctor.setCallback(callback);
	}

	void setOnReleasedCallback(UIElementCallback callback) {
		mOnReleasedFunctor.mUIElement = this;
		mOnReleasedFunctor.setCallback(callback);
	}

	void setOnTextChangedCallback(UIElementCallback callback) {
		mOnTextChangedFunctor.mUIElement = this;
		mOnTextChangedFunctor.setCallback(callback);
	}

	void setOnFocusLostCallback(UIElementCallback callback) {
		mOnFocusLostFunctor.mUIElement = this;
		mOnFocusLostFunctor.setCallback(callback);
	}

	void setComponentsCache();

	virtual void setVisibility(bool visibility);
	bool isVisible();

	virtual void toggle() {};
};

}

