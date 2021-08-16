#pragma once

#include "Core/ObjectBase.hpp"
#include "UI/UIElement.hpp"
#include <string>

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*);

public:
	void init(EditorController* editorController);
};