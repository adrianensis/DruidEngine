#include "ToolsBar.hpp"

#include "EditorStyles.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Material/MaterialManager.hpp"

#include "../EditorController.hpp"
#include "EditorUIGroups.hpp"

void ToolsBar::init(EditorController* editorController)
{
	Super::init(editorController);

	UIBuilder uiBuilder;

	uiBuilder.
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1.0f, 0.8f)).
	setLayer(0).
	setGroup(EditorUIGroups::smToolsBar).
	setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.1f, 0.1f));

	uiBuilder.
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Cursor.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		getEditorController()->getBrush().setModeSelect();
	});

	uiBuilder.
	getUIElement<UIToggleButton>()->simulateClick();

	uiBuilder.
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Pencil.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		getEditorController()->getBrush().setModePaint();
	});

	uiBuilder.
	nextRow();

	uiBuilder.
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Eraser.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		getEditorController()->getBrush().setModeErase();
	});
}

void ToolsBar::setVisibility(bool visible)
{
	Super::setVisibility(visible);

	UI::getInstance()->getOrCreateGroup(EditorUIGroups::smToolsBar).setVisibility(visible);
}