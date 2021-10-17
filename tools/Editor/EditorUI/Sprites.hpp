#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "Scene/GameObject.hpp"
#include "EditorUI/Grid.hpp"

class UIButton;
class UIPanel;
class Material;

class EditorController;

CLASS(SpriteEditorObject, GameObject)
{
public:
    DESERIALIZE();
};

CLASS(Sprites, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
	PRI(SStr, SpritesSelectorUIGroup, NONE)
    PRI(SStr, SpritesUIGroup, NONE)
    PRI(SStr, SpritePreviewUIGroup, NONE)
	PRI(SStr, AtlasUIGroup, NONE)
    PRI(SStr, FramesUIGroup, NONE)
    PRI(SLst(GameObject*), Frames, NONE)
    PRI(SLst(GameObject*), Sprites, NONE)

	PRI(bool, IsVisible, GET)

    PRI(u32, SpritesCount, NONE)

    PRI(GameObject*, CurrentSpriteSelector, NONE)
    PRI(GameObject*, CurrentSprite, NONE)

    PRI(UIPanel*, SpritePreview, NONE)

    PRI(GameObject*, SelectedAtlasTile, NONE)

    PRI(Material*, CurrentAtlasMaterial, GET)

    PRI(bool, IsRecording, GET)

    PRI(SStr, CurrentAnimationName, NONE)

private:

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

public:

	Sprites();
    ~Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
    
    void saveSprites();

    SERIALIZE();
    DESERIALIZE();
};