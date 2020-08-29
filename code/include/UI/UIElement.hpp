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

class UIElement: public GameObject {

private:

	FunctorVoid mOnPressedFunctor;
	FunctorVoid mOnReleasedFunctor;

	FunctorVoid mOnFocusLostFunctor;

	Collider* mCollider;
	Renderer* mRenderer;

	bool mPressed;

	bool mConsumeInput;

	std::string mInputString;

	UIGroup* mGroup;

public:

	DE_CLASS(UIElement, GameObject);

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

	void onFocusLost();
	void onFocus();

	void setOnPressedCallback(std::function<void()> callback) {
		mOnPressedFunctor.setCallback(callback);
	}

	void setOnReleasedCallback(std::function<void()> callback) {
		mOnReleasedFunctor.setCallback(callback);
	}

	void setOnTextChangedCallback(std::function<void()> callback) {
		mOnFocusLostFunctor.setCallback(callback);
	}

	Collider* getCollider() const {
		return mCollider;
	};

	Renderer* getRenderer() const {
		return mRenderer;
	};


	bool getPressed() const {
		return mPressed;
	};

	const std::string& getInputString() const {
		return mInputString;
	};

	void setConsumeInput(bool consumeInput) {
		mConsumeInput = consumeInput;
	};

	bool isConsumeInput() const {
		return mConsumeInput;
	};

	void setComponentsCache();

	const UIGroup* getGroup() const { return mGroup; };
	void setGroup(UIGroup* group) { mGroup = group; };

	virtual void setVisibility(bool visibility);
	bool isVisible();

	virtual void toggle() {};

};

} /* namespace DE */

#endif /* DE_UIELEMENT_H */
