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
	PRI(SStr, AtlasUIGroup, NONE)
	PRI(bool, IsVisible, GET)
    PRI(Grid, Grid, NONE) // TODO: use this grid to create sprites

    PRI(UIPanel*, CurrentSpritePreview, NONE)
    PRI(SLst(UIPanel*), FramesPreview, NONE)

private:

	void createAtlas(Material* material);
	void createAtlasSelectors();
    void createSpriteMenu();

    void addSprite();
    void removeSprite();

    void addAnimation();
    void removeAnimation();

    void addFrame();
    void removeFrame();

public:

	Sprites();

	void init(EditorController* editorController);
	void toggle();
	void setVisible(bool visible);
};