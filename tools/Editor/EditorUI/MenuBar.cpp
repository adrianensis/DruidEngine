#include "MenuBar.hpp"
#include "InfoBar.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "../EditorController.hpp"

void MenuBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,1)).
	setLayer(5).
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
	setText("Layer").
	create<UIDropdown>().
	getUIElement<UIDropdown>();


	FOR_RANGE(i, 0, 10)
	{
		u32 layer = i;
		std::string layerStr = "Layer " + std::to_string(layer);

		layerDropdown->addOption(layerStr, [&, this, layer](UIElement *uiElement)
		{
			mEditorController->setLayer(layer);
			mEditorController->getInfoBar().setLayer(mEditorController->getLayer());
		});
	}

	UI::getInstance()->getUIBuilder().
	setText("Grid").
	create<UIButton>().
	getUIElement<UIButton>()->
	setOnPressedCallback([&, this](UIElement *uiElement){
		mEditorController->setDrawGrid(!mEditorController->getDrawGrid());
	});
}