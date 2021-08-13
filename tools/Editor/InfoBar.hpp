#pragma once

#include "Core/ObjectBase.hpp"

class EditorController;
class UIText;

CLASS(InfoBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*)
	
	PRI(FPS, NONE, UIText*)
	PRI(Layer, NONE, UIText*)

public:
	void init(EditorController* editorController);

	void setLayer(u32 layer);
	void setFPS(f32 fps);
};