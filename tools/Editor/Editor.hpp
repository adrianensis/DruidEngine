#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>

#include "Brush.hpp"

CLASS(Editor, Script)
{
	PRI(Brush, NONE, Brush)
public:
	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};