#include "UI/UIButton.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Events/EventsManager.hpp"

namespace DE {

UIButton::UIButton() : UIElement() {
	mLabel = nullptr;
}

UIButton::~UIButton() = default;

void UIButton::init() {
	UIElement::init();
}

void UIButton::onDestroy() {
	if(mLabel) {
		getScene()->removeGameObject(mLabel);
	}

	UIElement::onDestroy();
}

void UIButton::onLabelDestroy() {
	mLabel = nullptr;
}

void UIButton::setText(const String &text) {

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

			DE_SUBSCRIBE_TO_EVENT(EventOnDestroy, mLabel, this, [&](const Event* event){
				onLabelDestroy();
			});
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

}
