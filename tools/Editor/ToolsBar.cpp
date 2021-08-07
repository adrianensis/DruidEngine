#include "ToolsBar.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "EditorController.hpp"

void ToolsBar::addButton(const std::string &name, UIElementCallback callback) const
{
	UI::getInstance()->getUIBuilder().
	setText(name).
	create<UIButton>().
	getUIElement<UIButton>()->
	setOnPressedCallback(callback);
}

void ToolsBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL).
	setPosition(Vector2(-1,0.8)).
	setLayer(0).
	setSize(Vector2(0.5f, 0.05f));

	addButton("Atlas", [&](UIElement* uiElement){
		mEditorController->getAtlas().toggle();
	});
}