#include "MenuBar.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "EditorController.hpp"

void MenuBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,1)).
	setLayer(0).
	setAdjustSizeToText(true).
	setTextSize(Vector2(0.025f, 0.05f)).
	setSize(Vector2(0.5f, 0.05f));

	UI::getInstance()->getUIBuilder().
	setText("File").
	create<UIDropdown>().
	getUIElement<UIDropdown>()->
	addOption("New", [&](UIElement *uiElement)
	{

	}).
	addOption("Open", [&](UIElement *uiElement)
	{
		mEditorController->loadScene();
	}).
	addOption("Save", [&](UIElement *uiElement)
	{
		mEditorController->saveScene();
	});

	UIDropdown* layerDropdown = UI::getInstance()->getUIBuilder().
	setText("Layer 0").
	create<UIDropdown>().
	getUIElement<UIDropdown>();


	FOR_RANGE(i, 0, 10)
	{
		std::string layerStr = std::to_string(i);

		layerDropdown->addOption(layerStr, [&, layerDropdown, layerStr](UIElement *uiElement)
		{
			layerDropdown->setText(std::string("Layer ") + layerStr);
			ECHO("AHHHHHHHH " + layerStr)
		});
	}
}