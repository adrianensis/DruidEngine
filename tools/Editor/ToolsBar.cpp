#include "ToolsBar.hpp"

#include "EditorStyles.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Material/MaterialManager.hpp"

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
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,0.8)).
	setLayer(0).
	setGroup("ToolsBar").
	setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.15f, 0.15f));

	/*addButton("Atlas", [&](UIElement* uiElement){
		mEditorController->getAtlas().toggle();
	});*/

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(false).
	setText("").
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Atlas.png")).
	create<UIToggleButton>().
	getUIElement<UIToggleButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		mEditorController->getAtlas().setVisible(true);
	});

	UI::getInstance()->getUIBuilder().
	getUIElement<UIToggleButton>()->
	setOnReleasedCallback([&](UIElement* uiElement){
		mEditorController->getAtlas().setVisible(false);
	});

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
	nextRow();

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