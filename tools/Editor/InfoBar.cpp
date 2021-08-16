#include "InfoBar.hpp"

#include "UI/UI.hpp"

#include "EditorController.hpp"

#define CREATE_INFO_FIELD(BaseName)\
m##BaseName = createInfoField(#BaseName);

void InfoBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,-0.95)).
	setLayer(0).
	setAdjustSizeToText(false).
	setTextSize(Vector2(0.025f, 0.05f)).
	setSize(Vector2(0.25f, 0.05f));

	CREATE_INFO_FIELD(FPS)
	CREATE_INFO_FIELD(Layer)
	CREATE_INFO_FIELD(BrushSize)
}

UIText* InfoBar::createInfoField(const std::string &name)
{
	return UI::getInstance()->getUIBuilder().
	setText(name).
	create<UIText>().
	getUIElement<UIText>();
}