#include "UIDropdown.hpp"
#include "UI.hpp"
#include "UIBuilder.hpp"
#include "UIText.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "List.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIDropdown::UIDropdown() : UIButton() {
	mButtons = nullptr;
}

// ---------------------------------------------------------------------------

UIDropdown::~UIDropdown() {
	Memory::free<List<UIButton*>>(mButtons);
}

// ---------------------------------------------------------------------------

void UIDropdown::init() {

	UIElement::init();

	mButtons = Memory::allocate<List<UIButton*>>();
	mButtons->init();

	subscribeToMouseButtonEvents();
}

void UIDropdown::addOption(const std::string& label) {

	Vector3 lastPosition(0,0,0);

	if(!mButtons->isEmpty()){
		lastPosition = mButtons->getLast().get()->getTransform()->getWorldPosition();
	}

	UI::getInstance()->getBuilder()->saveData()->
			setPosition(Vector2(0.0f, - getTransform()->getScale().y * (mButtons->getLength() + 1)))->
			setSize(Vector2(getTransform()->getScale().x * RenderContext::getAspectRatio(), getTransform()->getScale().y))->
			setText(label)->
			setLayer(getRenderer()->getLayer() + 1)->
			setIsAffectedByLayout(false)->
			create(UIElementType::BUTTON);

	UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->getUIElement();

	mButtons->pushBack(button);

	button->setVisibility(false);

	button->getTransform()->setParent(getTransform());

	UI::getInstance()->getBuilder()->restoreData();
}

void UIDropdown::toggle() {
	FOR_LIST(it, mButtons){
		it.get()->setVisibility(!it.get()->isVisible());
	}
}

} /* namespace DE */
