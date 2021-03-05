#include "UI/UIList.hpp"

namespace DE {

UIList::UIList() : UIElement() {

}

UIList::~UIList() = default;

void UIList::init() {
	UIElement::init();

	subscribeToMouseButtonEvents();
}

void UIList::setText(const String &text) {

}

}
