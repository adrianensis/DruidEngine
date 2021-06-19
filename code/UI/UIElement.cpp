#include "UI/UIElement.hpp"

#include "Graphics/Renderer.hpp"

#include "Events/EventsManager.hpp"
#include "Input/InputEvents.hpp"
#include "Events/Event.hpp"

#include "Input/Input.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Maths/Vector2.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "UI/UI.hpp"

void UIElement::init() {
	GameObject::init();

	mPressed = false;
	mConsumeInput = true;

	//subscribeToMouseButtonEvents();
}

void UIElement::onDestroy() {
	GameObject::onDestroy();
	UNSUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this);

	if(hasFocus()){
		UI::getInstance()->setFocusedElement(nullptr);
	}
}

void UIElement::subscribeToKeyEvents() {
	SUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});

	SUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});
}

void UIElement::subscribeToCharEvents() {
	SUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this, [this](const Event* event){
		if(isActive()){
			// TODO : boolean to enable or disable : can receive char input?
			onChar(((const InputEventChar*)event)->mChar);
		}
	});
}

void UIElement::subscribeToMouseButtonEvents() {
	SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event* event){
		if(isActive()){
			//ECHO("InputEventMouseButtonPressed")
			const InputEventMouseButtonPressed* e = (const InputEventMouseButtonPressed*)event;

			if(e->mButton == GLFW_MOUSE_BUTTON_LEFT){
				onPressed();
			}
		}
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this, [this](const Event* event){
		if(isActive()){

		}
	});
}

void UIElement::subscribeToEnterEvent() {
	SUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this, [this](const Event* event){
		if(isActive()){
			onFocusLost(); // TODO : call something more generic
		}
	});
}

void UIElement::subscribeToEscEvent() {
	SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [this](const Event* event){
		if(isActive()){
			onFocusLost(); // TODO : call something more generic
		}
	});
}

bool UIElement::hasFocus() const {
	return this == UI::getInstance()->getFocusedElement();
}

void UIElement::onChar(char character) {
	if(hasFocus()){
		mInputString += character;
		setText(mInputString);
	}
}

void UIElement::onFocusLost() {
	if(hasFocus()){
		UI::getInstance()->setFocusedElement(nullptr);
		mOnFocusLostFunctor.execute();
	}
}

void UIElement::onFocus() {

}


void UIElement::onPressed() {

	//Collider* collider = getCollider();
	// TODO : boolean to enable or disable : can be pressed?

	if (mRenderer->isActive()){

		//mRenderer->setColor(Vector4(1,0,0,0.7f));

		Vector2 screenMousePosition(Input::getInstance()->getMousePosition());
		Vector2 worldMousePosition = Vector2(
				RenderEngine::getInstance()->getCamera()->screenToWorld(screenMousePosition));

		//collider->getBoundingBox(true); // force regenerate bounding box
		Vector2 mousePosition = getTransform()->getAffectedByProjection() ? worldMousePosition : screenMousePosition;
		
		bool clickOk = Geometry::testRectanglePoint(
			mRenderer->getVertices()[0],
			getTransform()->getScale().x,
			getTransform()->getScale().y,
			mousePosition, 0);

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

void UIElement::onReleased() {

	// TODO : boolean to enable or disable : can be pressed?

	mPressed = false;
	//mRenderer->setColor(Vector4(0.0f,0.0f,0.0f,1.0f));
	mOnReleasedFunctor.execute();
}

void UIElement::setComponentsCache() {
	mRenderer = getFirstComponent<Renderer>();
	//mCollider = getFirstComponent<Collider>();
}

void UIElement::setVisibility(bool visibility) {
	setIsActive(visibility);
}

bool UIElement::isVisible(){
	return isActive();
}