#pragma once

#include "Core/Core.hpp"
#include "UI/UIText.hpp"

class EditorController;

#define INFO_BAR_MEMBER(BaseName, ...)\
PRI(UIText*, BaseName, GET)\
public:\
void set##BaseName(__VA_ARGS__ new##BaseName)\
{\
	m##BaseName->setText(#BaseName " " + std::to_string(new##BaseName));\
}

CLASS(InfoBar, ObjectBase)
{
	PRI(EditorController*, EditorController, NONE)
	
	INFO_BAR_MEMBER(Layer, u32)
	INFO_BAR_MEMBER(FPS, u32)
	INFO_BAR_MEMBER(BrushSize, u32)

private:
	UIText* createInfoField(CNS SStr &name);

public:
	void init(EditorController* editorController);
};