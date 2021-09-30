#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE);

public:
	void init(EditorController* editorController);
};