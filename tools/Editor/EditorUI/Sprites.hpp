#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"
#include "EditorUI/Grid.hpp"

class GameObject;
class UIButton;
class UIPanel;
class Material;

class EditorController;

CLASS(Sprites, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
	PRI(SStr, SpritesSelectorUIGroup, NONE)
    PRI(SStr, SpritesUIGroup, NONE)
    PRI(SStr, SpritePreviewUIGroup, NONE)
	PRI(SStr, AtlasUIGroup, NONE)
	PRI(bool, IsVisible, GET)

    PRI(u32, SpritesCount, NONE)

    PRI(GameObject*, CurrentSprite, NONE)

private:

    void loadSprites();

	void createAtlas(Material* material);
	void createAtlasSelectors();
    void createSpriteMenu();

    void createSpritePreview(GameObject* sprite);

    void addSprite();
    void removeSprite();

    void addAnimation(const SStr& name);
    void removeAnimation();

    void addFrame();
    void removeFrame();

public:

	Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
};