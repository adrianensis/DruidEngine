#include "Sprites.hpp"
#include "../EditorController.hpp"

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

#include "Core/Time/TimerManager.hpp"

Sprites::Sprites()
{
	mSpritesSelectorUIGroup = "spritesSelector";
	mSpritesUIGroup = "sprites";
	mIsVisible = true;
}

void Sprites::init(EditorController* editorController)
{
	mEditorController = editorController;

	createSpritesSelectors();

	toggle();
}

void Sprites::createSpritesSelectors()
{
	UI::getInstance()->getUIBuilder().
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::VERTICAL).
		setPosition(Vector2(-0.8f, 0.5f)).
		setText("").
		setSize(Vector2(0.1f, 0.1f)).
		setLayer(0).
		setGroup(mSpritesSelectorUIGroup).
		setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditor>());

	FOR_LIST(it, mEditorController->getEditorConfig().at("atlases"))
	{
		Material* material = MaterialManager::getInstance()->loadMaterial((*it).get<SStr>());

		UIButton* atlasSelector = UI::getInstance()->getUIBuilder().
			setMaterial(material).
			create<UIButton>().
			getUIElement<UIButton>();

		atlasSelector->setOnPressedCallback([&, material](UIElement* uiElement)
		{
			TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
			{
				UI::getInstance()->removeElementsFromGroup(mSpritesUIGroup);
				createSprites(material);
			});
		});
	}

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();

	// TODO : Load sprites from disk
	UI::getInstance()->getUIBuilder().
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(0.3f, 0.5f)).
		setText("").
		setSize(Vector2(0.1f, 0.1f)).
		setLayer(0).
		setGroup(mSpritesSelectorUIGroup);

	FOR_RANGE(i, 0, 4)
	{
		FOR_RANGE(j, 0, 4)
		{
			UIButton* sprites = UI::getInstance()->getUIBuilder().
				create<UIButton>().
				getUIElement<UIButton>();

			sprites->setOnPressedCallback([&](UIElement* uiElement)
			{

			});
		}

		UI::getInstance()->getUIBuilder().nextRow();
	}

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();

	UI::getInstance()->getUIBuilder().
		setLayout(UILayout::VERTICAL).
		setAdjustSizeToText(true).
		setTextSize(Vector2(0.025f, 0.05f)).
		setPosition(Vector2(0.7f, 0.5f)).
		setLayer(0).
		setGroup(mSpritesSelectorUIGroup);

	UI::getInstance()->getUIBuilder().
		setText("Add Sprite").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{

		});

	UI::getInstance()->getUIBuilder().
		setText("Edit Sprite").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{

		});

	UI::getInstance()->getUIBuilder().
		setText("Remove Sprite").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{

		});

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();
}

void Sprites::createSprites(Material* material)
{
	f32 tileSize = 0.06f;

	Vector2 screenOffset = Vector2(tileSize/2.0f, tileSize/2.0f);

	Vector2 spritesSize = Vector2(8,16);
	Vector2 spritesTextureSize = Vector2(1.0f / spritesSize.x, 1.0f / spritesSize.y);

	Vector2 panelSize = spritesSize * tileSize;

	UI::getInstance()->getUIBuilder().
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-panelSize.x/2.0f, panelSize.y/2.0f)).
		setText("").
		setSize(Vector2(tileSize, tileSize)).
		setLayer(0).
		setGroup(mSpritesUIGroup).
		setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditor>());

	FOR_RANGE(i, 0, spritesSize.y)
	{
		FOR_RANGE(j, 0, spritesSize.x)
		{
			UIButton* tile = UI::getInstance()->getUIBuilder().
				setMaterial(material).
				create<UIButton>().
				getUIElement<UIButton>();

			Renderer* renderer = tile->getRenderer();
			renderer->setRegion(Rectangle(j / spritesSize.x, (spritesSize.y - i - 1) / spritesSize.y, spritesTextureSize.x, spritesTextureSize.y));

			tile->setOnPressedCallback([&](UIElement* uiElement)
			{
				this->mEditorController->getBrush().onTileSelectedFromAtlas(uiElement);
			});
		}

		UI::getInstance()->getUIBuilder().nextRow();
	}

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();
}

void Sprites::toggle()
{
	mIsVisible = !mIsVisible;

	setVisible(mIsVisible);
}

void Sprites::setVisible(bool visible)
{
	mIsVisible = visible;
	UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsVisible);
	UI::getInstance()->setGroupVisibility(mSpritesSelectorUIGroup, mIsVisible);
}
