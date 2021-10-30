#pragma once

#include "Core/Core.hpp"


class UIElement;

CLASS(UIGroup, ObjectBase)
{
	PUB_M(SStr, Name, GETREF_CONST)
	PUB_M(bool, Visible, GET)
	PUB_M(SLst(UIElement *), UIElements, GETREF_CONST);

PUB
	void init();
};