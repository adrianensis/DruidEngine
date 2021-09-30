#pragma once

#include "Core/Core.hpp"

class EditorController;

CLASS(MenuBar, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
public:
	void init(EditorController* editorController);
};