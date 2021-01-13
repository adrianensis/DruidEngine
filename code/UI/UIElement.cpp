#include "UI/UIElement.hpp"

#include "Physics/Collider.hpp"
#include "Graphics/Renderer.hpp"
#include "Containers/List.hpp"

#include "Events/EventsManager.hpp"
#include "Input/InputEvents.hpp"
#include "Events/Event.hpp"

#include "Input/Input.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Maths/Vector2.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "UI/UI.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIElement::UIElement() : GameObject() {
	mCollider = nullptr;
	mRenderer = nullptr;
	mPressed = false;
	mConsumeInput = true;
	mGroup = nullptr;
}

// ---------------------------------------------------------------------------

UIElement::~UIElement() {

}

void UIElement::init() {
	GameObject::init();
	subscribeToMouseButtonEvents();
}

void UIElement::onDestroy() {
	GameObject::onDestroy();
	DE_UNSUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this);
	DE_UNSUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this);
}

// ---------------------------------------------------------------------------

void UIElement::subscribeToKeyEvents() {
	DE_SUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});

	DE_SUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});
}

void UIElement::subscribeToCharEvents() {
	DE_SUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this, [this](const Event* event){
		if(isActive()){
			// TODO : boolean to enable or disable : can receive char input?
			onChar(((const InputEventChar*)event)->mChar);
		}
	});
}

void UIElement::subscribeToMouseButtonEvents() {
	DE_SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event* event){
		if(isActive()){
			//ECHO("InputEventMouseButtonPressed")
			const InputEventMouseButtonPressed* e = (const InputEventMouseButtonPressed*)event;

			if(e->mButton == GLFW_MOUSE_BUTTON_LEFT){
				onPressed();
			}
		}
	});

	DE_SUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});
}

void UIElement::subscribeToEnterEvent() {
	DE_SUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this, [this](const Event* event){
		if(isActive()){
			onFocusLost(); // TODO : call something more generic
		}
	});
}

void UIElement::subscribeToEscEvent() {
	DE_SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [this](const Event* event){
		if(isActive()){
			onFocusLost(); // TODO : call something more generic
		}
	});
}

// ---------------------------------------------------------------------------

bool UIElement::hasFocus() const {
	return this == UI::getInstance()->getFocusedElement();
}

void UIElement::onChar(c8 character) {
	if(hasFocus()){
		mInputString += character;
		setText(mInputString);
	}
}

void UIElement::onFocusLost() {
	if(hasFocus()){
		mOnFocusLostFunctor.execute();
		UI::getInstance()->setFocusedElement(nullptr);
	}
}

void UIElement::onFocus() {

}


// ---------------------------------------------------------------------------

void UIElement::onPressed() {

	Collider* collider = getCollider();
	// TODO : boolean to enable or disable : can be pressed?

	if (collider){

		Vector2 screenMousePosition(Input::getInstance()->getMousePosition());
		Vector2 worldMousePosition = Vector2(
				RenderEngine::getInstance()->getCamera()->screenToWorld(screenMousePosition));

		collider->getBoundingBox(true); // force regenerate bounding box
		Vector2 mousePosition = getTransform()->getAffectedByProjection() ? worldMousePosition : screenMousePosition;
		bool clickOk = collider->testPoint(mousePosition) == ColliderStatus::STATUS_PENETRATION;

		if(clickOk){
			mPressed = true;
			//mRenderer->setColor(Vector4(1.0f,1.0f,0.0f,1.0f));
			mOnPressedFunctor.execute();

			if(!hasFocus()) {
				UIElement* lastFocusedElement = UI::getInstance()->getFocusedElement();

				if(lastFocusedElement) {
					lastFocusedElement->onFocusLost();
				}

				UI::getInstance()->setFocusedElement(this);
				onFocus();
			}

			if (getConsumeInput()) {
				Input::getInstance()->clearMouseButton();
			}
		}
		/*else {
			UIElement* lastFocusedElement = UI::getInstance()->getFocusedElement();

			if(lastFocusedElement) {
				lastFocusedElement->onFocusLost();
			}

			UI::getInstance()->setFocusedElement(nullptr);
		}*/
	}
}

// ---------------------------------------------------------------------------

void UIElement::onReleased() {

	// TODO : boolean to enable or disable : can be pressed?

	mPressed = false;
	//mRenderer->setColor(Vector4(0.0f,0.0f,0.0f,1.0f));
	mOnReleasedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::setComponentsCache() {
	mRenderer = getFirstComponent<Renderer>();
	mCollider = getFirstComponent<Collider>();
}

// ---------------------------------------------------------------------------

void UIElement::setVisibility(bool visibility) {
	getRenderer()->setIsActive(visibility);
	setIsActive(visibility);
}

bool UIElement::isVisible(){
	return getRenderer()->isActive();
}

} /* namespace DE */
