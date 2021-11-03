#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Scene/GameObject.hpp"
#include "EditorUI/Grid.hpp"
#include "EditorUIElement.hpp"

class UIButton;
class UIPanel;
class Material;

class Sprites: public EditorUIElement
{
    GENERATE_METADATA(Sprites)    
    PRI std::list<GameObject*> mFrames = {};
    PRI std::list<GameObject*> mSprites = {};

    PRI u32 mSpritesCount = {};

    PRI GameObject* mCurrentSpriteSelector = {};
    PRI GameObject* mCurrentSprite = {};

    PRI UIPanel* mSpritePreview = {};

    PRI GameObject* mSelectedAtlasTile = {};

    PRI Material* mCurrentAtlasMaterial = {}; GET(CurrentAtlasMaterial)

    PRI bool mIsRecording = {}; GET(IsRecording)

    PRI std::string mCurrentAnimationName = {};

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

    SERIALIZE();
    DESERIALIZE();
};