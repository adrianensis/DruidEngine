#pragma once

#include "Core/Core.hpp"


class UIElement;

CLASS(UIGroup, ObjectBase)
{
	PUB(SStr, Name, GETREF_CONST)
	PUB(bool, Visible, GET)
	PUB(SLst(UIElement *), UIElements, GETREF_CONST);

public:
	void init();
};