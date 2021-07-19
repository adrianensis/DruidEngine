#include "Atlas.hpp"

#include "Maths/Vector3.hpp"

#include "Input/Input.hpp"
#include "Input/InputEvents.hpp"
#include "Events/EventsManager.hpp"

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
	mIsAtlasShow = true;
}

Atlas::~Atlas()
{

}

void Atlas::init()
{
	createAtlas(MaterialManager::getInstance()->loadMaterial("resources/tiles.png"));

	toggle();

	SUBSCRIBE_TO_EVENT(InputEventKeyTab, nullptr, this, [this](const Event *event)
	{
		toggle();
	});
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

	FOR_RANGE(i, 0, atlasSize.y){
		FOR_RANGE(j, 0, atlasSize.x){

			UIButton* tile = (UIButton*) UI::getInstance()->getUIBuilder().
					create<UIButton>().
					getUIElement();

			Renderer* renderer = tile->getRenderer();
			renderer->setMaterial(material);
			renderer->setRegion(Rectangle(j / atlasSize.x, (atlasSize.y - i - 1) / atlasSize.y, atlasTextureSize.x, atlasTextureSize.y));

			tile->setOnPressedCallback([&](UIElement* uiElement) {

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
	mIsAtlasShow = !mIsAtlasShow;

	UI::getInstance()->setGroupVisibility(mAtlasUIGroup, mIsAtlasShow);
	UI::getInstance()->setGroupVisibility(mAtlasSelectorUIGroup, mIsAtlasShow);
	//UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsAtlasShow);
}