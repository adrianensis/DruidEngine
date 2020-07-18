#include "UIElement.hpp"

#include "Collider.hpp"
#include "Renderer.hpp"
#include "List.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIElement::UIElement() : GameObject() {
	mCollider = nullptr;
	mRenderer = nullptr;
	mPressed = false;
	mConsumeInput = true;
}

// ---------------------------------------------------------------------------

UIElement::~UIElement() = default;

// ---------------------------------------------------------------------------

void UIElement::inputCharCallback(c8 character) {

	mInputString += character;
	setText(mInputString);
}

// ---------------------------------------------------------------------------

void UIElement::inputCloseCallback() {
	mOnTextChangedFunctor.execute();
	mInputString.clear();
}

// ---------------------------------------------------------------------------

void UIElement::onPressed() {
	mPressed = true;
	//mRenderer->setColor(Vector4(1.0f,1.0f,0.0f,1.0f));
	mOnPressedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::onReleased() {
	mPressed = false;
	//mRenderer->setColor(Vector4(0.0f,0.0f,0.0f,1.0f));
	mOnReleasedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::setComponentsCache() {
	mRenderer = getComponents<Renderer>() ? getComponents<Renderer>()->get(0) : nullptr;
	mCollider = getComponents<Collider>() ? getComponents<Collider>()->get(0) : nullptr;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
