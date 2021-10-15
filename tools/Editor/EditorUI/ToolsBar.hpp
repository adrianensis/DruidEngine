#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE);
    PRI(SStr, UIGroupName, NONE)
	PRI(bool, IsVisible, GET)

public:
	void init(EditorController* editorController);
    void toggle();
};