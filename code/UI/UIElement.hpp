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

	UIElement* mUIElement = nullptr;

	FunctorUIElement():Functor<UIElementCallback>() {};
	~FunctorUIElement() override {};

	virtual void execute() override {
		if (mCallback)
			mCallback(mUIElement);
	}

};

class UIElement: public GameObject {

protected:

	FunctorUIElement mOnPressedFunctor;
	FunctorUIElement mOnReleasedFunctor;

	FunctorUIElement mOnTextChangedFunctor;
	FunctorUIElement mOnFocusLostFunctor;

	Collider* mCollider;
	Renderer* mRenderer;

	bool mPressed;

	bool mConsumeInput;

	std::string mInputString;

	UIGroup* mGroup;

public:

	DE_CLASS(UIElement, GameObject)

	DE_GET(Renderer)
	DE_GET(Collider)
	DE_GET(InputString)
	DE_GET_SET(ConsumeInput)
	DE_GET_SET(Group)
	DE_GET(Pressed)

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
