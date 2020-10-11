#include "UITextEditable.hpp"
#include "UI.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UITextEditable::UITextEditable() : UIText() {
}

// ---------------------------------------------------------------------------

UITextEditable::~UITextEditable() = default;

// ---------------------------------------------------------------------------


void UITextEditable::init() {
	UIText::init();

	subscribeToMouseButtonEvents();
	subscribeToCharEvents();
	subscribeToEnterEvent();
}

// ---------------------------------------------------------------------------

void UITextEditable::onFocusLost() {
	if(hasFocus()){
		UIText::onFocusLost();

		mOnTextChangedFunctor.execute();
		mInputString.clear();

		UIText::onFocusLost();
	}
}

} /* namespace DE */
