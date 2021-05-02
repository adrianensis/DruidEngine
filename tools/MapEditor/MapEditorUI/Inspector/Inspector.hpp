#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "../StringsUI.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

class GameObject;
class UIButton;
class UIText;
class UITextEditable;
template<class T> class Array;

class MapEditor;

/**
 * @brief
 */
class Inspector : public ObjectBase{

private:

	

	// LAYERS
	Array<UIButton*>* mLayerButtons = nullptr;

	// INSPECTOR
	UIText* mTextBoxTag = nullptr;
	UIText* mTextInspectorX = nullptr;
	UIText* mTextInspectorY = nullptr;
	UITextEditable* mTextBoxSizeX = nullptr;
	UITextEditable* mTextBoxSizeY = nullptr;
	UITextEditable* mTextBoxColliderPosX = nullptr;
	UITextEditable* mTextBoxColliderPosY = nullptr;
	UITextEditable* mTextBoxColliderSizeX = nullptr;
	UITextEditable* mTextBoxColliderSizeY = nullptr;
	UIButton* mButtonInspectorCollider = nullptr;

	MapEditor* mMapEditor;

	void createTileInspector();
	void createLayersBar();

public:

	GENERATE_METADATA(Inspector);

	Inspector();
	virtual ~Inspector() override;

	void init(MapEditor *mapEditor);

	void setTileScaleFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex);
	void setTileColliderPositionFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex);
	void setTileColliderScaleFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex);

	void update();
	void updateInspectorOnSelectTile();

	void selectLayer(u32 layer);
};
}

