#include "InfoBar.hpp"

#include "UI/UI.hpp"

#include "../EditorController.hpp"

#define CREATE_INFO_FIELD(BaseName, builder)\
m##BaseName = createInfoField(#BaseName, builder);

void InfoBar::init(EditorController* editorController)
{
	EditorUIElement::init(editorController);

	UIBuilder uiBuilder;

	uiBuilder.
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,-0.95)).
	setLayer(0).
	setAdjustSizeToText(false).
	setSize(Vector2(0.25f, 0.05f));

	CREATE_INFO_FIELD(FPS, uiBuilder)
	CREATE_INFO_FIELD(Layer, uiBuilder)
	CREATE_INFO_FIELD(BrushSize, uiBuilder)
}

UIText* InfoBar::createInfoField(const std::string &name, UIBuilder& uiBuilder)
{
	return uiBuilder.
	setText(name).
	create<UIText>().
	getUIElement<UIText>();
}