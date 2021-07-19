#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>

#include "Brush.hpp"
#include "Grid.hpp"
#include "Atlas.hpp"

CLASS(Editor, Script)
{
	PRI(Brush, NONE, Brush)
	PRI(Grid, NONE, Grid)
	PRI(Atlas, NONE, Atlas)

public:
	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void drawGrid() const;
};