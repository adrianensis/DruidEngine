#include "Atlas.hpp"
#include "EditorController.hpp"

#include "Maths/Vector3.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/Transform.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIStyle.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"

Atlas::Atlas()
{
	mAtlasSelectorUIGroup = "atlasSelector";
	mAtlasUIGroup = "atlas";
	mIsVisible = true;
}

Atlas::~Atlas()
{

}

void Atlas::init(EditorController* editorController)
{
	mEditorController = editorController;

	createAtlas(MaterialManager::getInstance()->loadMaterial("resources/tiles.png"));

	toggle();
}

void Atlas::createAtlas(Material* material)
{
	UI::getInstance()->getUIBuilder().
	setGroup(mAtlasUIGroup);

	f32 tileSize = 0.06f;

	Vector2 screenOffset = Vector2(tileSize/2.0f, tileSize/2.0f);

	Vector2 atlasSize = Vector2(8,16);
	Vector2 atlasTextureSize = Vector2(1.0f / atlasSize.x, 1.0f / atlasSize.y);

	Vector2 panelSize = atlasSize * tileSize;

	UI::getInstance()->getUIBuilder().
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-panelSize.x/2.0f, panelSize.y/2.0f)).
		setText("").
		setSize(Vector2(tileSize, tileSize)).
		setLayer(0).
		setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleAtlasButton>());

	FOR_RANGE(i, 0, atlasSize.y)
	{
		FOR_RANGE(j, 0, atlasSize.x)
		{
			UIButton* tile = (UIButton*) UI::getInstance()->getUIBuilder().
					create<UIButton>().
					getUIElement();

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(material);
			renderer->setRegion(Rectangle(j / atlasSize.x, (atlasSize.y - i - 1) / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y));

			tile->setOnPressedCallback([&](UIElement* uiElement)
			{
				this->mEditorController->getBrush().onTileSelectedFromAtlas(uiElement);
			});
		}

		UI::getInstance()->getUIBuilder().nextRow();
	}

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	setGroup("").
	restoreStyle();
}

void Atlas::toggle()
{
	mIsVisible = !mIsVisible;

	UI::getInstance()->setGroupVisibility(mAtlasUIGroup, mIsVisible);
	UI::getInstance()->setGroupVisibility(mAtlasSelectorUIGroup, mIsVisible);
	//UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsVisible);
}