#pragma once

#include "Core/Core.hpp"
#include "UI/UIElement.hpp"

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI_M(EditorController*, EditorController, NONE);
    PRI_M(SStr, UIGroupName, NONE)
	PRI_M(bool, IsVisible, GET)

PUB
	void init(EditorController* editorController);
    void toggle();
};