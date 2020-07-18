#ifndef DE_UIELEMENT_H
#define DE_UIELEMENT_H

#include "GameObject.hpp"
#include "Functor.hpp"
#include "RenderContext.hpp"
#include <string>

namespace DE {

class Collider;
class Renderer;

class UIElement: public GameObject {

private:

	Functor mOnPressedFunctor;
	Functor mOnReleasedFunctor;

	Functor mOnTextChangedFunctor;

	Collider* mCollider;
	Renderer* mRenderer;

	bool mPressed;

	bool mConsumeInput;

	std::string mInputString;

public:

	DE_CLASS(UIElement, GameObject);

	void inputCharCallback(c8 character);
	void inputCloseCallback();

	virtual void setText(const std::string &text) = 0;

	void onPressed();
	void onReleased();

	void setOnPressedCallback(std::function<void()> callback) {
		mOnPressedFunctor.setCallback(callback);
	}

	void setOnReleasedCallback(std::function<void()> callback) {
		mOnReleasedFunctor.setCallback(callback);
	}

	void setOnTextChangedFunctor(std::function<void()> callback) {
		mOnTextChangedFunctor.setCallback(callback);
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

};

} /* namespace DE */

#endif /* DE_UIELEMENT_H */
