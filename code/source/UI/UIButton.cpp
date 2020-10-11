#include "UIButton.hpp"
#include "UI.hpp"
#include "UIBuilder.hpp"
#include "UIText.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIButton::UIButton() : UIElement() {
	mLabel = nullptr;
}

// ---------------------------------------------------------------------------

UIButton::~UIButton() = default;

// ---------------------------------------------------------------------------

void UIButton::init() {
	UIElement::init();

	subscribeToMouseButtonEvents();
}

void UIButton::setText(const std::string &text) {

	if(text.length() > 0){
		f32 fontSize = 0.04f;
		f32 fontOffset = 0.02f;
		Vector3 buttonScale = getTransform()->getScale();

		if (!mLabel) {

			UI::getInstance()->getBuilder()->saveData()->
					setAdjustSizeToText(false)->
					setPosition(Vector2(0, 0))->
					setSize(Vector2(fontSize/1.5f, fontSize))->
					setText(text)->
					setLayer(getRenderer()->getLayer() + 1)->
					setIsAffectedByLayout(false)->
					create(UIElementType::TEXT);

			mLabel = (UIText*) UI::getInstance()->getBuilder()->getUIElement();

			UI::getInstance()->getBuilder()->restoreData();

			mLabel->getTransform()->setParent(getTransform());
		}

		mLabel->setText(text);

		mLabel->getTransform()->setLocalPosition(
				Vector3(-buttonScale.x / 2.0f /*+ fontSize / 2.0f*/ + fontOffset, 0, 0));
	}
}

void UIButton::setVisibility(bool visibility) {
	UIElement::setVisibility(visibility);

	if(mLabel) mLabel->setVisibility(visibility);
}

} /* namespace DE */
