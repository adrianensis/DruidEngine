#ifndef DE_UIELEMENT_H
#define DE_UIELEMENT_H

#include "GameObject.hpp"
#include "Functor.hpp"
#include "RenderContext.hpp"
#include <string>

namespace DE {

class Collider;
class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = std::function<void(UIElement* uiElement)>;

class FunctorUIElement : public Functor<UIElementCallback> {
public:
	DE_GENERATE_METADATA(FunctorUIElement, Functor<UIElementCallback>);

	DE_PUBLIC_M(UIElement, UIElement*)

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

class UIElement: public GameObject {

protected:

	DE_M(OnPressedFunctor, FunctorUIElement);
	DE_M(OnReleasedFunctor, FunctorUIElement);

	DE_M(OnTextChangedFunctor, FunctorUIElement);
	DE_M(OnFocusLostFunctor, FunctorUIElement);

	DE_M_GET(Renderer, Renderer*)
	DE_M_GET(Collider, Collider*)
	DE_M_GET(InputString, std::string)
	DE_M_GET_SET(ConsumeInput, bool)
	DE_M_GET_SET(Group, UIGroup*)
	DE_M_GET(Pressed, bool)

public:

	DE_CLASS(UIElement, GameObject)

	virtual void init();
	virtual void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseButtonEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	void onChar(c8 character);

	virtual void setText(const std::string &text) = 0;

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

} /* namespace DE */

#endif /* DE_UIELEMENT_H */
