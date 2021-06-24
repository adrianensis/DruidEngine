#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

#include "Maths/Vector4.hpp"

CLASS(UIStyle, ObjectBase)
{
	PUB(Color, NONE, Vector4);
	PUB(ColorSelected, NONE, Vector4);
	PUB(ColorHovered, NONE, Vector4);
};

CLASS(UIStyleManager, ObjectBase), SINGLETON(UIStyleManager)
{
	PRI(DefaultStyle, GETREF_CONST, UIStyle);

public:

	void init();
};