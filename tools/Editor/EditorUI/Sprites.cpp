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
#include "UI/UIPanel.hpp"

#include "Core/Time/TimerManager.hpp"

#include "Events/EventsManager.hpp"
#include "Input/InputEvents.hpp"
#include "Events/Event.hpp"

Sprites::Sprites()
{
	mSpritesSelectorUIGroup = "spritesSelector";
	mAtlasUIGroup = "atlas";
    mSpritesUIGroup = "sprites";
    mSpritePreviewUIGroup = "mSpritePreview";
    mFramesUIGroup = "frames";
	mIsVisible = true;
    mIsRecording = false;
    mCurrentAtlasMaterial = nullptr;
}

Sprites::~Sprites()
{
    FOR_LIST(it, mSprites)
    {
        (*it)->destroy();
        delete *it;
    }
}

void Sprites::init(EditorController* editorController)
{
	mEditorController = editorController;

	createAtlasSelectors();
    createSpriteMenu();
    loadSprites();
	toggle();
}

void Sprites::toggle()
{
	mIsVisible = !mIsVisible;
	setVisible(mIsVisible);
}

void Sprites::setVisible(bool visible)
{
	mIsVisible = visible;
	UI::getInstance()->setGroupVisibility(mAtlasUIGroup, mIsVisible);
	UI::getInstance()->setGroupVisibility(mSpritesSelectorUIGroup, mIsVisible);
    UI::getInstance()->setGroupVisibility(mSpritesUIGroup, mIsVisible);
    UI::getInstance()->setGroupVisibility(mSpritePreviewUIGroup, mIsVisible);
    UI::getInstance()->setGroupVisibility(mFramesUIGroup, mIsVisible);
}

void Sprites::createAtlasSelectors()
{
	UI::getInstance()->getUIBuilder().
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-1.0f, 0.8f)).
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
				UI::getInstance()->destroyAllElementsInGroup(mAtlasUIGroup);
				createAtlas(material);
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

	/*FOR_RANGE(i, 0, 4)
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
	}*/

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();
}

void Sprites::createSpriteMenu()
{
	UI::getInstance()->getUIBuilder().
		setLayout(UILayout::VERTICAL).
		setAdjustSizeToText(true).
		setTextSize(Vector2(0.025f, 0.05f)).
		setPosition(Vector2(0.8f, 0.9f)).
		setLayer(0).
		setGroup(mSpritesSelectorUIGroup);

    UI::getInstance()->getUIBuilder().
		setText("Sprites").
		create<UIText>();

    UI::getInstance()->getUIBuilder().
		setText("Name").
		create<UIEditableText>().
        getUIElement<UIEditableText>();

	UI::getInstance()->getUIBuilder().
		setText("Add").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{
            addSprite();
		});

    UI::getInstance()->getUIBuilder().
		setText("Animations").
		create<UIText>();

    UIEditableText* editableTextAnimation = UI::getInstance()->getUIBuilder().
		setText("Name").
		create<UIEditableText>().
        getUIElement<UIEditableText>();

	UIButton* recordButton = UI::getInstance()->getUIBuilder().
		setText("Record").
		create<UIToggleButton>().
		getUIElement<UIToggleButton>();

        recordButton->setReleaseOnSameGroupPressed(false);

		recordButton->setOnPressedCallback([&](UIElement* uiElement)
		{
            mIsRecording = true;
		});

        recordButton->setOnReleasedCallback([&](UIElement* uiElement)
		{
            mIsRecording = false;
		});

	UI::getInstance()->getUIBuilder().
		setText("Clear").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{
            mFrames.clear();
            refreshFrames();
		});

    UI::getInstance()->getUIBuilder().
		setText("Add").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&, editableTextAnimation](UIElement* uiElement)
		{
            addAnimation(editableTextAnimation->getInputString());
		});

	UI::getInstance()->getUIBuilder().
	restoreSeparatorSize().
	restoreMaterial().
	setGroup("").
	restoreStyle();
}

void Sprites::createAtlas(Material* material)
{
    mCurrentAtlasMaterial = material;
    mSelectedAtlasTile = nullptr;

	f32 tileSize = 0.06f;

	Vector2 spritesSize = Vector2(8,16);
	Vector2 spritesTextureSize = Vector2(1.0f / spritesSize.x, 1.0f / spritesSize.y);

	Vector2 panelSize = spritesSize * tileSize;

	UI::getInstance()->getUIBuilder().
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-panelSize.x/2.0f - 0.5f, panelSize.y/2.0f)).
		setText("").
		setSize(Vector2(tileSize, tileSize)).
		setLayer(0).
		setGroup(mAtlasUIGroup).
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
                mSelectedAtlasTile = uiElement;

                if(mIsRecording)
                {
                    addFrame();
                }

				this->mEditorController->getBrush().onTileSelectedFromAtlas(uiElement);
			});
		}

		UI::getInstance()->getUIBuilder().nextRow();
	}

	UI::getInstance()->getUIBuilder().
    setText("").
    restoreSeparatorSize().
    restoreMaterial().
    restoreStyle().
	setGroup("");
}

void Sprites::loadSprites()
{
    ConfigObject configMap;

	configMap.readFromJsonFile("tools/Editor/config/sprites.json");

    deserialize(configMap.getJson());

    refreshSprites();
}

void Sprites::saveSprites()
{
    ConfigObject configMap;
	configMap.init();

	JSON json;
	serialize(json);

	configMap.setJson(json);

	configMap.writeToJsonFile("tools/Editor/config/sprites.json");
}

void Sprites::refreshSpritePreview(GameObject* sprite)
{
    Renderer* renderer = sprite->getFirstComponent<Renderer>();

    f32 tileSize = 0.3f;

    UI::getInstance()->destroyAllElementsInGroup(mSpritePreviewUIGroup);

     UI::getInstance()->getUIBuilder().
        setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(0.1f, 0.9f)).
        setTextSize(Vector2(0.025f, 0.05f)).
        setAdjustSizeToText(true).
        setText("Sprite Name").
		setSeparatorSize(0).
		setGroup(mSpritePreviewUIGroup).
		setLayer(0).
        create<UIText>().
        getUIElement<UIText>();

    UI::getInstance()->getUIBuilder().nextRow();    

    mSpritePreview = UI::getInstance()->getUIBuilder().
		setAdjustSizeToText(false).
		setText("").
		setSize(Vector2(tileSize, tileSize)).
		setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditor>()).
        setMaterial(renderer->getMaterial()).
        create<UIPanel>().
        getUIElement<UIPanel>();

    mSpritePreview->getRenderer()->setRegion(renderer->getRegion());

    UI::getInstance()->getUIBuilder().
		setLayout(UILayout::HORIZONTAL).
		setAdjustSizeToText(true).
		setTextSize(Vector2(0.025f / 1.2f, 0.05f / 1.2f)).
		setPosition(Vector2(0.4f, 0.9f)).
		setLayer(0);

    UI::getInstance()->getUIBuilder().
    restoreMaterial().
    restoreStyle().
    restoreSeparatorSize();

    UI::getInstance()->getUIBuilder().
		setText("Animations").
		create<UIText>();

    UI::getInstance()->getUIBuilder().nextRow();

    FOR_MAP(it, renderer->getAnimations())
    {
        mSpritePreview->getRenderer()->addAnimation(it->first, it->second);

        SStr animationName = it->first;

        UI::getInstance()->getUIBuilder().
        setText(animationName).
        create<UIButton>().
        getUIElement<UIButton>()->
        setOnPressedCallback([&, sprite, renderer, animationName](UIElement* uiElement)
        {
            renderer->setAnimation(animationName);

            mCurrentAnimationName = animationName;

            TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
            {
                refreshSpritePreview(sprite);
            });
        });

        UI::getInstance()->getUIBuilder().
        setAdjustSizeToText(true).
        setText("-").
        create<UIButton>().
        getUIElement<UIButton>()->
        setOnPressedCallback([&, sprite, renderer, animationName](UIElement* uiElement)
        {
            renderer->removeAnimation(animationName);
            TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
			{
                refreshSpritePreview(sprite);
			});
        });

        UI::getInstance()->getUIBuilder().nextRow();
    }

    Renderer* previewRenderer = mSpritePreview->getFirstComponent<Renderer>();
    previewRenderer->setAnimation(mCurrentAnimationName);
}

void Sprites::addSprite()
{
    if(mCurrentAtlasMaterial)
    {
        // NOTE: this sprite game objects are not added to the scene, just needed to hold sprite info.
        GameObject* sprite = NEW(GameObject);
        sprite->init();

        Renderer* renderer = NEW(Renderer);
        renderer->init();
        renderer->setMesh(Mesh::getRectangle());
        renderer->setLayer(0);
        renderer->setMaterial(mCurrentAtlasMaterial);

        sprite->addComponent<Renderer>(renderer);

        mSprites.push_back(sprite);

        refreshSprites();
    }
}

void Sprites::removeSprite()
{
    if(mCurrentAtlasMaterial)
    {
        //refreshSprites();
    }
}

void Sprites::refreshSprites()
{
    UI::getInstance()->destroyAllElementsInGroup(mSpritesUIGroup);

    mCurrentAnimationName.clear();

    UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1.0f, -0.8f)).
	setLayer(0).
	setGroup(mSpritesUIGroup).
	setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.1f, 0.1f));

	FOR_LIST(it, mSprites)
    {
        Renderer* spriteRenderer = (*it)->getFirstComponent<Renderer>();

        UI::getInstance()->getUIBuilder().
        setAdjustSizeToText(false).
        setText("").
        setMaterial(spriteRenderer->getMaterial()).
        create<UIButton>().
        getUIElement<UIButton>()->
        setOnPressedCallback([&, sprite = *it](UIElement* uiElement)
        {
            mCurrentSpriteSelector = uiElement;
            mCurrentSprite = sprite;
            refreshSpritePreview(sprite);
        });
    }

    UI::getInstance()->getUIBuilder().
    setText("").
    restoreSeparatorSize().
    restoreMaterial().
    restoreStyle().
	setGroup("");
}

void Sprites::addAnimation(const SStr& name)
{
    if(mCurrentSprite)
    {
        Renderer* renderer = mCurrentSprite->getFirstComponent<Renderer>();
        Renderer* rendererSpriteSelector = mCurrentSpriteSelector->getFirstComponent<Renderer>();

        Renderer* firstFrameRenderer = mFrames.front()->getFirstComponent<Renderer>();

        Animation animation = Animation::create(
            mFrames.size(),
            true,
            false,
            firstFrameRenderer->getRegion().getLeftTop(),
            firstFrameRenderer->getRegion().getSize().x,
            firstFrameRenderer->getRegion().getSize().y,
            6
        );

        renderer->addAnimation(name, animation);
        rendererSpriteSelector->addAnimation(name, animation);
        refreshSpritePreview(mCurrentSprite);
    }
}

void Sprites::removeAnimation()
{

}

void Sprites::addFrame()
{
    if(mSpritePreview)
    {
        mFrames.push_back(mSelectedAtlasTile);
        refreshFrames();   
    }
}

void Sprites::refreshFrames()
{
    if(mCurrentSprite)
    {   
        UI::getInstance()->destroyAllElementsInGroup(mFramesUIGroup);

        UI::getInstance()->getUIBuilder().
        setLayout(UILayout::VERTICAL).
        setPosition(Vector2(-1.0f, -0.6f)).
        setLayer(0).
        setGroup(mFramesUIGroup);

        FOR_LIST(it, mFrames)
        {
            Renderer* renderer = (*it)->getFirstComponent<Renderer>();

            UI::getInstance()->getUIBuilder().
            setAdjustSizeToText(false).
            setText("").
            setSize(Vector2(0.05f, 0.05f)).
            setStyle(&UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorToolsBar>()).
            setMaterial(renderer->getMaterial()).
            create<UIPanel>().
            getUIElement<UIPanel>()->
            getRenderer()->setRegion(renderer->getRegion());

            UI::getInstance()->getUIBuilder().
            restoreMaterial().
            restoreStyle();

            UI::getInstance()->getUIBuilder().
            setAdjustSizeToText(true).
            setText("-").
            create<UIButton>().
            getUIElement<UIButton>()->
            setOnPressedCallback([&, frame = *it](UIElement* uiElement)
            {
                mFrames.remove(frame);
                TimerManager::getInstance()->setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
                {
                    refreshFrames();
                });
            });

            UI::getInstance()->getUIBuilder().nextColumn();
        }

        UI::getInstance()->getUIBuilder().
        setText("").
        restoreSeparatorSize().
        restoreMaterial().
        restoreStyle().
        setGroup("");
    }
}

SERIALIZE_IMPL(Sprites)
{
    DO_SERIALIZE_LIST("sprites", mSprites);
}

DESERIALIZE_IMPL(Sprites)
{
    DO_DESERIALIZE_LIST("sprites", mSprites, [](const JSON &json)
    {
        GameObject *gameObject = INSTANCE_BY_NAME(json["class"], GameObject);
        gameObject->init();
        return gameObject;
    })
}

DESERIALIZE_IMPL(SpriteEditorObject)
{

}