#pragma once

#include "Core/Core.hpp"
#include "EditorUIElement.hpp"

class UIBuilder;

#define INFO_BAR_MEMBER(BaseName, ...)\
PRI UIText* m##BaseName; GET(BaseName)\
PUB \
void set##BaseName(__VA_ARGS__ new##BaseName)\
{\
	m##BaseName->setText(#BaseName " " + std::to_string(new##BaseName));\
}

class InfoBar: public EditorUIElement
{
    GENERATE_METADATA(InfoBar)	
	INFO_BAR_MEMBER(Layer, u32)
	INFO_BAR_MEMBER(FPS, u32)
	INFO_BAR_MEMBER(BrushSize, u32)

PRI
	UIText* createInfoField(const std::string &name, UIBuilder& uiBuilder);

PUB
	void init(EditorController* editorController) override;
};