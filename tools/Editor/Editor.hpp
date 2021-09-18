#pragma once

#include "Scripting/Script.hpp"
#include "EditorController.hpp"

CLASS(Editor, Script)
{
	PRI(EditorController, GET, EditorController)

public:
	void init() OVR;
	void firstUpdate() OVR;
	void update() OVR;
	void terminate() OVR;

	void drawGrid() CNS;
};