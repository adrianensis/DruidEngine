#include "UIButton.hpp"
#include "UI.hpp"
#include "UIText.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIButton::UIButton() :
		UIElement() {
	mLabel = nullptr;
}

// ---------------------------------------------------------------------------

UIButton::~UIButton() = default;

// ---------------------------------------------------------------------------

void UIButton::setLabel(const std::string &label) {

	f32 fontSize = 0.04f;
	f32 fontOffset = 0.02f;
	Vector3 buttonScale = getTransform()->getScale();

	if (!mLabel) {
		mLabel = UI::getInstance()->createText(getScene(), Vector2(0, 0), Vector2(fontSize, fontSize), label,
				getRenderer()->getLayer() + 1);
		mLabel->getTransform()->setParent(getTransform());
	}

	mLabel->setText(label);

	mLabel->getTransform()->setLocalPosition(
			Vector3(-buttonScale.x / 2.0f /*+ fontSize / 2.0f*/ + fontOffset, 0, 0));
}

} /* namespace DE */
