#include "EditorBuilder.hpp"

#include "Core/TimeUtils.hpp"

#include "Log/Log.hpp"

#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"
#include "UI/UIDropdown.hpp"

#include "Scene/GameObjectBuilder.hpp"

namespace DE {

EditorBuilder::EditorBuilder() : ObjectBase(), Singleton<EditorBuilder>(){

}

EditorBuilder::~EditorBuilder(){

}

UIElement* EditorBuilder::createPanel(const Vector2& position, const Vector2 &size) {
	return UI::getInstance()->getBuilder()->
	setLayout(UILayout::VERTICAL)->
	setPosition(position)->
	setLayer(mUILayer-1)->
	setAdjustSizeToText(false)->
	setSize(size)->
	create(UIElementType::PANEL)->
	getUIElement();
}

UIText* EditorBuilder::createLabel(StringRef text) {
	return (UITextEditable*) UI::getInstance()->getBuilder()->
		setText(text)->
		setAdjustSizeToText(true)->
		create(UIElementType::TEXT)->
		getUIElement();
}

UITextEditable* EditorBuilder::createTextBoxSimple(StringRef text, std::function<void(UIElement* uiElement)> onTextChangedCallback) {

	UITextEditable* textEditable = (UITextEditable*) UI::getInstance()->getBuilder()->
		setText(text)->
		setAdjustSizeToText(false)->
		setSize(Vector2(UI::getInstance()->getDefaultFontSize().x * 4, UI::getInstance()->getDefaultFontSize().y))->
		create(UIElementType::TEXTEDITABLE)->
		getUIElement();

	textEditable->setOnTextChangedCallback(onTextChangedCallback);

	return textEditable;
}

UITextEditable* EditorBuilder::createTextBoxLabeled(StringRef textLabel, StringRef text, std::function<void(UIElement* uiElement)> onTextChangedCallback) {

	createLabel(textLabel);
	UITextEditable* textEditable = createTextBoxSimple(text, onTextChangedCallback);

	UI::getInstance()->getBuilder()->nextRow();
	return textEditable;
}

UITextEditableVector2 EditorBuilder::createTextBoxVector2(StringRef textLabel,
		std::function<void(UIElement* uiElement)> onTextChangedCallbackX, std::function<void(UIElement* uiElement)> onTextChangedCallbackY) {

	createLabel(textLabel);

	UITextEditableVector2 textEditableVector2;
	String initValue = "0";
	textEditableVector2.TextEditableX = createTextBoxSimple(initValue, onTextChangedCallbackX);
	textEditableVector2.TextEditableY = createTextBoxSimple(initValue, onTextChangedCallbackY);

	UI::getInstance()->getBuilder()->nextRow();

	return textEditableVector2;
}

UIButton* EditorBuilder::createButton(StringRef textLabel, UIElementCallback onPressedCallback) {
	UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->
		setText(textLabel)->
		create(UIElementType::BUTTON)->
		getUIElement();

	button->setOnPressedCallback(onPressedCallback);

	return button;
}

UIButton* EditorBuilder::createRadialButton(StringRef textLabel, UIElementCallback onPressedCallback) {
	createLabel(textLabel);
	UIButton* button = createButton("[ ]", onPressedCallback);

	UI::getInstance()->getBuilder()->nextRow();

	return button;
}

UIDropdown* EditorBuilder::createDropdown(StringRef textLabel) {
	return (UIDropdown*) UI::getInstance()->getBuilder()->
		setText(textLabel)->
		create(UIElementType::DROPDOWN)->
		getUIElement();
}

}
