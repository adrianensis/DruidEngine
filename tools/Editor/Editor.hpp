#pragma once

#include "Scripting/Script.hpp"
#include "EditorController.hpp"

class Editor: public Script
{
    GENERATE_METADATA(Editor)
	PRI_M(EditorController, EditorController, GET)

PUB
	void init() override;
	void firstUpdate() override;
	void update() override;
	void terminate() override;

	void drawGrid() const;
};