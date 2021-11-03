#include "MenuBar.hpp"
#include "InfoBar.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "../EditorController.hpp"

void MenuBar::init(EditorController* editorController)
{
	EditorUIElement::init(editorController);
	
	UIBuilder uiBuilder;

	uiBuilder.
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,1)).
	setLayer(5).
	setAdjustSizeToText(true).
	setSize(Vector2(0.5f, 0.05f));

	uiBuilder.
	setText("File").
	create<UIDropdown>().
	getUIElement<UIDropdown>()->
	addOption("New", [&](UIElement *uiElement)
	{

	}).
	addOption("Open", [&](UIElement *uiElement)
	{
		getEditorController()->loadScene();
	}).
	addOption("Save", [&](UIElement *uiElement)
	{
		getEditorController()->saveScene();
	});

	uiBuilder.
	setText("Grid").
	create<UIButton>().
	getUIElement<UIButton>()->
	setOnPressedCallback([&, this](UIElement *uiElement){
		getEditorController()->setDrawGrid(!getEditorController()->getDrawGrid());
	});

	uiBuilder.
	setText("Sprites").
	create<UIButton>().
	getUIElement<UIButton>()->
	setOnPressedCallback([&, this](UIElement *uiElement){
		getEditorController()->getSprites().toggle();
        getEditorController()->getLayersBar().toggle();
        getEditorController()->getToolsBar().toggle();
	});
}