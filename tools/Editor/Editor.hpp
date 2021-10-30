#pragma once

#include "Scripting/Script.hpp"
#include "EditorController.hpp"

CLASS(Editor, Script)
{
	PRI_M(EditorController, EditorController, GET)

PUB
	void init() override;
	void firstUpdate() override;
	void update() override;
	void terminate() override;

	void drawGrid() const;
};