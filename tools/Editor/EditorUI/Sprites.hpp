#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Scene/GameObject.hpp"
#include "EditorUI/Grid.hpp"
#include "EditorUIElement.hpp"

class UIButton;
class UIPanel;
class Material;

CLASS(Sprites, EditorUIElement)
{    
    PRI_M(SLst(GameObject*), Frames, NONE)
    PRI_M(SLst(GameObject*), Sprites, NONE)

    PRI_M(u32, SpritesCount, NONE)

    PRI_M(GameObject*, CurrentSpriteSelector, NONE)
    PRI_M(GameObject*, CurrentSprite, NONE)

    PRI_M(UIPanel*, SpritePreview, NONE)

    PRI_M(GameObject*, SelectedAtlasTile, NONE)

    PRI_M(Material*, CurrentAtlasMaterial, GET)

    PRI_M(bool, IsRecording, GET)

    PRI_M(SStr, CurrentAnimationName, NONE)

PRI

    void loadSprites();

	void createAtlas(Material* material);
	void createAtlasSelectors();
    void createSpriteMenu();

    void refreshSpritePreview(GameObject* sprite);

    void addSprite();
    void removeSprite();
    void refreshSprites();

    void addAnimation(const SStr& name);
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