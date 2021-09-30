#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

class EditorController;

CLASS(LayersBar, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE);

public:
	void init(EditorController* editorController);
};