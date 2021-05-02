#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "../StringsUI.hpp"
#include "Scene/MapElement/MapElement.hpp"

namespace DE {

class GameObject;
class UIButton;
class Material;
template<class T> class Array;

class MapEditor;

class Atlas : public ObjectBase{

private:

	

	String mAtlasSelectorUIGroup = "atlasSelector";
	String mAtlasUIGroup = "atlas";
	Array<UIButton*>* mAtlasButtons = nullptr;
	bool mIsAtlasShow = true;
	GameObject* mAtlasBackground = nullptr;

	MapEditor* mMapEditor;

	MapElementData_Tile mMapElementData_Tile; 

	void createAtlasSelector();
	void createAtlas(u32 index, Material* material);

public:

	GENERATE_METADATA(Atlas);

	Atlas();
	virtual ~Atlas() override;

	void init(MapEditor *mapEditor);

	void toggleAtlas();
};
}

