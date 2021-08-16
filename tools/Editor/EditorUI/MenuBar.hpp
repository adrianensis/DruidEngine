#pragma once

#include "Core/ObjectBase.hpp"

class EditorController;

CLASS(MenuBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
public:
	void init(EditorController* editorController);
};