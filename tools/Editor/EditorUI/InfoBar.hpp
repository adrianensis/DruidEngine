#pragma once

#include "Core/ObjectBase.hpp"
#include "UI/UIText.hpp"

class EditorController;

#define INFO_BAR_MEMBER(BaseName, ...)\
PRI(BaseName, GET, UIText*)\
public:\
void set##BaseName(__VA_ARGS__ new##BaseName)\
{\
	m##BaseName->setText(#BaseName " " + std::to_string(new##BaseName));\
}

CLASS(InfoBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	
	INFO_BAR_MEMBER(Layer, u32)
	INFO_BAR_MEMBER(FPS, u32)
	INFO_BAR_MEMBER(BrushSize, u32)

private:
	UIText* createInfoField(const std::string &name);

public:
	void init(EditorController* editorController);
};