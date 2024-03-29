#include "ToolsBar.hpp"

#include "EditorStyles.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Material/MaterialManager.hpp"

#include "../EditorController.hpp"

void ToolsBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,0.8)).
	setLayer(0).
	setGroup("ToolsBar").
	setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.1f, 0.1f));

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Cursor.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		mEditorController->getBrush().setModeSelect();
	});

	UI::getInstance()->getUIBuilder().
	getUIElement<UIToggleButton>()->simulateClick();

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Pencil.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		mEditorController->getBrush().setModePaint();
	});

	UI::getInstance()->getUIBuilder().
	nextRow();

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Eraser.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		mEditorController->getBrush().setModeErase();
	});

	UI::getInstance()->getUIBuilder().
	restoreStyle().
	setGroup("").
	restoreMaterial();
}