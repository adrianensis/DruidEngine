#include "LayersBar.hpp"

#include "EditorStyles.hpp"

#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Material/MaterialManager.hpp"

#include "../EditorController.hpp"

void LayersBar::init(EditorController* editorController)
{
    mEditorController = editorController;

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL).
	setPosition(Vector2(0.94,0.8)).
	setLayer(0).
	setGroup("LayersBar").
	setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.07f, 0.07f)).
	//setAdjustSizeToText(true).
	//restoreMaterial().
	setMaterial(MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Layer.png")).
	setTextSize(Vector2(0.04f, 0.04f));

	FOR_RANGE(i, 0, 10)
	{
		u32 layer = i;
		std::string layerStr = std::to_string(layer);

		UI::getInstance()->getUIBuilder().
		setText(layerStr).
		create<UIToggleButton>().
		getUIElement<UIToggleButton>()->
		setOnPressedCallback([&, layer](UIElement* uiElement){
			mEditorController->setLayer(layer);
			mEditorController->getInfoBar().setLayer(mEditorController->getLayer());
		});
	}

	UI::getInstance()->getUIBuilder().
	restoreStyle().
	setGroup("").
	restoreMaterial();
}