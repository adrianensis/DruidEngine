#include "Sprites.hpp"
#include "../EditorController.hpp"

#include "Graphics/Graphics.hpp"

#include "Scenes/Scenes.hpp"

#include "EditorUIGroups.hpp"

Sprites::Sprites()
{
    mIsRecording = false;
    mCurrentAtlasMaterial = nullptr;
}

Sprites::~Sprites()
{
    FOR_LIST(it, mSprites)
    {
        (*it)->destroy();
        DELETE(*it);
    }
}

void Sprites::init(EditorController* editorController)
{
	EditorUIElement::init(editorController);

	createAtlasSelectors();
    createSpriteMenu();
    loadSprites();
	toggle();
}

void Sprites::setVisibility(bool visible)
{
	EditorUIElement::setVisibility(visible);

	UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smAtlas).setVisibility(getIsVisible());
	UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smSpritesSelector).setVisibility(getIsVisible());
    UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smSprites).setVisibility(getIsVisible());
    UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smSpritePreview).setVisibility(getIsVisible());
    UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smFrames).setVisibility(getIsVisible());
}

void Sprites::createAtlasSelectors()
{
    UIBuilder uiBuilder;

	uiBuilder.
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-1.0f, 0.8f)).
		setSize(Vector2(0.1f, 0.1f)).
		setGroup(EditorUIGroups::smSpritesSelector).
		setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditor>());

	FOR_LIST(it, getEditorController()->getEditorConfig().at("atlases"))
	{
		Material* material = MaterialManager::getInstance().loadMaterial((*it).get<std::string>());

		UIButton* atlasSelector = uiBuilder.
			setMaterial(material).
			create<UIButton>().
			getUIElement<UIButton>();

		atlasSelector->setOnPressedCallback([&, material](UIElement* uiElement)
		{
			TimerManager::getInstance().setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
			{
				UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smAtlas).destroyAllUIElements();
				createAtlas(material);
			});
		});
	}

	

	// TODO : Load sprites from disk
	uiBuilder.
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(0.3f, 0.5f)).
		setSize(Vector2(0.1f, 0.1f)).
		setGroup(EditorUIGroups::smSpritesSelector);

	/*FOR_RANGE(i, 0, 4)
	{
		FOR_RANGE(j, 0, 4)
		{
			UIButton* sprites = uiBuilder.
				create<UIButton>().
				getUIElement<UIButton>();

			sprites->setOnPressedCallback([&](UIElement* uiElement)
			{

			});
		}

		uiBuilder.nextRow();
	}*/
}

void Sprites::createSpriteMenu()
{
    UIBuilder uiBuilder;

	uiBuilder.
		setLayout(UILayout::VERTICAL).
		setAdjustSizeToText(true).
		setPosition(Vector2(0.8f, 0.9f)).
		setGroup(EditorUIGroups::smSpritesSelector);

    uiBuilder.
		setText("Sprites").
		create<UIText>();

    uiBuilder.
		setText("Name").
		create<UIEditableText>().
        getUIElement<UIEditableText>();

	uiBuilder.
		setText("Add").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{
            addSprite();
		});

    uiBuilder.
		setText("Animations").
		create<UIText>();

    UIEditableText* editableTextAnimation = uiBuilder.
		setText("Name").
		create<UIEditableText>().
        getUIElement<UIEditableText>();

	UIButton* recordButton = uiBuilder.
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

	uiBuilder.
		setText("Clear").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&](UIElement* uiElement)
		{
            mFrames.clear();
            refreshFrames();
		});

    uiBuilder.
		setText("Add").
		create<UIButton>().
		getUIElement<UIButton>()->
		setOnPressedCallback([&, editableTextAnimation](UIElement* uiElement)
		{
            addAnimation(editableTextAnimation->getInputString());
		});
}

void Sprites::createAtlas(Material* material)
{
    mCurrentAtlasMaterial = material;
    mSelectedAtlasTile = nullptr;

	f32 tileSize = 0.06f;

	Vector2 spritesSize = Vector2(8,16);
	Vector2 spritesTextureSize = Vector2(1.0f / spritesSize.x, 1.0f / spritesSize.y);

	Vector2 panelSize = spritesSize * tileSize;

    UIBuilder uiBuilder;

	uiBuilder.
		setSeparatorSize(0).
		setAdjustSizeToText(false).
		setLayout(UILayout::HORIZONTAL).
		setPosition(Vector2(-panelSize.x/2.0f - 0.5f, panelSize.y/2.0f)).
		setSize(Vector2(tileSize, tileSize)).
		setGroup(EditorUIGroups::smAtlas).
		setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditor>());

	FOR_RANGE(i, 0, spritesSize.y)
	{
		FOR_RANGE(j, 0, spritesSize.x)
		{
			UIButton* tile = uiBuilder.
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

				this->getEditorController()->getBrush().onTileSelectedFromAtlas(uiElement);
			});
		}

		uiBuilder.nextRow();
	}
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

    UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smSpritePreview).destroyAllUIElements();

    UIBuilder uiBuilder;

    uiBuilder.
        setLayout(UILayout::HORIZONTAL).
        setPosition(Vector2(0.1f, 0.9f)).
        setAdjustSizeToText(true).
        setText("Sprite Name").
        setSeparatorSize(0).
        setGroup(EditorUIGroups::smSpritePreview).
        create<UIText>().
        getUIElement<UIText>();

    uiBuilder.nextRow();    

    mSpritePreview = uiBuilder.
		setAdjustSizeToText(false).
		setSize(Vector2(tileSize, tileSize)).
		setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditor>()).
        setMaterial(renderer->getMaterial()).
        create<UIPanel>().
        getUIElement<UIPanel>();

    mSpritePreview->getRenderer()->setRegion(renderer->getRegion());

    uiBuilder.
    restoreAll();

    uiBuilder.
		setLayout(UILayout::HORIZONTAL).
		setAdjustSizeToText(true).
		setPosition(Vector2(0.4f, 0.9f)).
        setGroup(EditorUIGroups::smSpritePreview);

    uiBuilder.
		setText("Animations").
		create<UIText>();

    uiBuilder.nextRow();

    FOR_MAP(it, renderer->getAnimations())
    {
        mSpritePreview->getRenderer()->addAnimation(it->first, it->second);

        std::string animationName = it->first;

        uiBuilder.
        setText(animationName).
        create<UIButton>().
        getUIElement<UIButton>()->
        setOnPressedCallback([&, sprite, renderer, animationName](UIElement* uiElement)
        {
            renderer->setAnimation(animationName);

            mCurrentAnimationName = animationName;

            TimerManager::getInstance().setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
            {
                refreshSpritePreview(sprite);
            });
        });

        uiBuilder.
        setAdjustSizeToText(true).
        setText("-").
        create<UIButton>().
        getUIElement<UIButton>()->
        setOnPressedCallback([&, sprite, renderer, animationName](UIElement* uiElement)
        {
            renderer->removeAnimation(animationName);
            TimerManager::getInstance().setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
			{
                refreshSpritePreview(sprite);
			});
        });

        uiBuilder.nextRow();
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
    UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smSprites).destroyAllUIElements();

    mCurrentAnimationName.clear();

    UIBuilder uiBuilder;

    uiBuilder.
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1.0f, -0.8f)).
	setGroup(EditorUIGroups::smSprites).
	setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditorToolsBar>()).
	setSize(Vector2(0.1f, 0.1f));

	FOR_LIST(it, mSprites)
    {
        Renderer* spriteRenderer = (*it)->getFirstComponent<Renderer>();

        uiBuilder.
        setAdjustSizeToText(false).
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

    
}

void Sprites::addAnimation(const std::string& name)
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
        UIManager::getInstance().getOrCreateGroup(EditorUIGroups::smFrames).destroyAllUIElements();

        UIBuilder uiBuilder;

        uiBuilder.
        setLayout(UILayout::VERTICAL).
        setPosition(Vector2(-1.0f, -0.6f)).
        setGroup(EditorUIGroups::smFrames);

        FOR_LIST(it, mFrames)
        {
            Renderer* renderer = (*it)->getFirstComponent<Renderer>();

            uiBuilder.
            setAdjustSizeToText(false).
            setSize(Vector2(0.05f, 0.05f)).
            setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditorToolsBar>()).
            setMaterial(renderer->getMaterial()).
            create<UIPanel>().
            getUIElement<UIPanel>()->
            getRenderer()->setRegion(renderer->getRegion());

            uiBuilder.
            restoreMaterial().
            restoreStyle();

            uiBuilder.
            setAdjustSizeToText(true).
            setText("-").
            create<UIButton>().
            getUIElement<UIButton>()->
            setOnPressedCallback([&, frame = *it](UIElement* uiElement)
            {
                mFrames.remove(frame);
                TimerManager::getInstance().setTimer(0.1f, TimerDurationType::TIME_AMOUNT, [&]()
                {
                    refreshFrames();
                });
            });

            uiBuilder.nextColumn();
        }

        
    }
}

void Sprites::serialize(JSON &json) const
{
    DO_SERIALIZE_LIST("sprites", mSprites);
}

void Sprites::deserialize(const JSON &json)
{
    DO_DESERIALIZE_LIST("sprites", mSprites, [](const JSON &json)
    {
        GameObject *gameObject = INSTANCE_BY_NAME(json["class"], GameObject);
        gameObject->init();
        return gameObject;
    })
}