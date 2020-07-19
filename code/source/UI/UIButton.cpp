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

void UIButton::setText(const std::string &text) {

	if(mLabel || (!mLabel && text.length() > 0)){
		f32 fontSize = 0.04f;
		f32 fontOffset = 0.02f;
		Vector3 buttonScale = getTransform()->getScale();

		if (!mLabel) {

			UI::getInstance()->getBuilder()->saveData()->
					setPosition(Vector2(0, 0))->
					setSize(Vector2(fontSize, fontSize))->
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

} /* namespace DE */
