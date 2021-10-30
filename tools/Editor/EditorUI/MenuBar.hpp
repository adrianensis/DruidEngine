#pragma once

#include "Core/Core.hpp"

class EditorController;

CLASS(MenuBar, ObjectBase)
{
	PRI_M(EditorController*, EditorController, NONE)
PUB
	void init(EditorController* editorController);
};