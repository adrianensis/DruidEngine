#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"

class GameObject;
class UIButton;
class Material;

class EditorController;

CLASS(Atlas, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	PRI(AtlasSelectorUIGroup, NONE, std::string)
	PRI(AtlasUIGroup, NONE, std::string)
	PRI(IsVisible, GET, bool)

private:

	void createAtlas(Material* material);

public:

	Atlas();
	virtual ~Atlas() override;

	void init(EditorController* editorController);
	void toggle();
};