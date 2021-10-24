#pragma once

#include "Scripting/Script.hpp"
#include "EditorController.hpp"

CLASS(Editor, Script)
{
	PRI(EditorController, EditorController, GET)

public:
	void init() override;
	void firstUpdate() override;
	void update() override;
	void terminate() override;

	void drawGrid() const;
};