#pragma once

#include "Core/Core.hpp"


class UIElement;

CLASS(UIGroup, ObjectBase)
{
	PUB(Name, GETREF_CONST, SStr)
	PUB(Visible, GET, bool)
	PUB(UIElements, GETREF_CONST, SLst<UIElement *>);

public:
	void init();
};