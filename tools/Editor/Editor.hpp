#pragma once

#include "Scripting/Script.hpp"
#include "EditorController.hpp"

CLASS(Editor, Script)
{
	PRI(EditorController, GET, EditorController)

public:
	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void drawGrid() const;
};