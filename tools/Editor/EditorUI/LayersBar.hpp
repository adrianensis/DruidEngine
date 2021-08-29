#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"
#include <string>

class EditorController;

CLASS(LayersBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*);

public:
	void init(EditorController* editorController);
};