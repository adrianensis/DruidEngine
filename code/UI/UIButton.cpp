#include "UI/UIButton.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Scene/Transform.hpp"

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
}

void UIButton::setText(const std::string &text) {

	if(text.length() > 0){
		f32 fontOffset = UI::getInstance()->getDefaultFontSize().x;
		Vector3 buttonScale = getTransform()->getScale();

		if (!mLabel) {

			mLabel = (UIText*) UI::getInstance()->getBuilder()->saveData()->
				setAdjustSizeToText(true)->
				setPosition(Vector2(0, 0))->
				setText(text)->
				setLayer(getRenderer()->getLayer() + 1)->
				setIsAffectedByLayout(false)->
				create(UIElementType::TEXT)
				->getUIElement();

			UI::getInstance()->getBuilder()->restoreData();

			mLabel->getTransform()->setParent(getTransform());
		}

		mLabel->setText(text);

		mLabel->getTransform()->setLocalPosition(
				Vector3((-buttonScale.x + fontOffset) / 2.0f /*+ fontSize / 2.0f*/, 0, 0));
	}
}

void UIButton::setVisibility(bool visibility) {
	UIElement::setVisibility(visibility);

	if(mLabel) mLabel->setVisibility(visibility);
}

} /* namespace DE */
