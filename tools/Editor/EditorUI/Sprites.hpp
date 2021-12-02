#pragma once

#include "Core/Core.hpp"
#include "Scenes/Scenes.hpp"
#include "EditorUI/Grid.hpp"
#include "EditorUIElement.hpp"

class UIButton;
class UIPanel;
class Material;

class Sprites: public EditorUIElement
{
    GENERATE_METADATA(Sprites)    
    PRI std::list<GameObject*> mFrames;
    PRI std::list<GameObject*> mSprites;

    PRI u32 mSpritesCount = 0;

    PRI GameObject* mCurrentSpriteSelector = nullptr;
    PRI GameObject* mCurrentSprite = nullptr;

    PRI UIPanel* mSpritePreview = nullptr;

    PRI GameObject* mSelectedAtlasTile = nullptr;

    PRI Material* mCurrentAtlasMaterial = nullptr; GET(CurrentAtlasMaterial)

    PRI bool mIsRecording = false; GET(IsRecording)

    PRI std::string mCurrentAnimationName;

PRI

    void loadSprites();

	void createAtlas(Material* material);
	void createAtlasSelectors();
    void createSpriteMenu();

    void refreshSpritePreview(GameObject* sprite);

    void addSprite();
    void removeSprite();
    void refreshSprites();

    void addAnimation(const std::string& name);
    void removeAnimation();

    void addFrame();
    void refreshFrames();

PUB

	Sprites();
    ~Sprites();

	void init(EditorController* editorController) override;
	void setVisibility(bool visible) override;
    
    void saveSprites();

    virtual void serialize(JSON &json) const override;
    virtual void deserialize(const JSON &json) override;
};