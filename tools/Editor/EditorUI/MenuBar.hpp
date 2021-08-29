#pragma once

#include "Core/Core.hpp"

class EditorController;

CLASS(MenuBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
public:
	void init(EditorController* editorController);
};