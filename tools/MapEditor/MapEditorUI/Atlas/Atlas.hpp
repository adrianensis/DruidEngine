#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "../StringsUI.hpp"

namespace DE {

class GameObject;
class UIButton;
class Material;
template<class T> class Array;

class MapEditor;

class Atlas : public DE_Class{

private:

	const StringsUI mStringsUI;

	std::string mAtlasSelectorUIGroup = "atlasSelector";
	std::string mAtlasUIGroup = "atlas";
	Array<UIButton*>* mAtlasButtons = nullptr;
	bool mIsAtlasShow = true;
	GameObject* mAtlasBackground = nullptr;

	MapEditor* mMapEditor;

	void createAtlasSelector();
	void createAtlas(u32 index, Material* material);

public:

	DE_CLASS(Atlas)

	void init(MapEditor *mapEditor);

	void toggleAtlas();
};

}

