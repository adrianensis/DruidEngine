#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*);

public:
	void init(EditorController* editorController);
};