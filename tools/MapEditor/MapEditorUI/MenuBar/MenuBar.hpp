#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "../StringsUI.hpp"
#include "Scene/MapElement/MapElement.hpp"

namespace DE {

class MenuBar : public ObjectBase{

private:

	

	class MapEditor* mMapEditor;
	MapElementData_ActionPoint mMapElementData_ActionPoint;

	void createMapElement(StringRef materialPath);

public:

	GENERATE_METADATA(MenuBar);

	MenuBar();
	virtual ~MenuBar() override;

	void init(class MapEditor *mapEditor);

	void createMenuBar();
};
}

