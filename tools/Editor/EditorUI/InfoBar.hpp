#pragma once

#include "Core/Core.hpp"
#include "UI/UIText.hpp"

class EditorController;

#define INFO_BAR_MEMBER(BaseName, ...)\
PRI_M(UIText*, BaseName, GET)\
PUB \
void set##BaseName(__VA_ARGS__ new##BaseName)\
{\
	m##BaseName->setText(#BaseName " " + std::to_string(new##BaseName));\
}

CLASS(InfoBar, ObjectBase)
{
	PRI_M(EditorController*, EditorController, NONE)
	
	INFO_BAR_MEMBER(Layer, u32)
	INFO_BAR_MEMBER(FPS, u32)
	INFO_BAR_MEMBER(BrushSize, u32)

PRI
	UIText* createInfoField(const SStr &name);

PUB
	void init(EditorController* editorController);
};