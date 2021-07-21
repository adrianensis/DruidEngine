#pragma once

#include "UI/UIStyle.hpp"

CLASS(UIStyleEditor, UIStyle)
{
public:
	UIStyleEditor()
	{
		
	}
};

CLASS(UIStyleEditorBrushPreview, UIStyleEditor)
{
public:
	UIStyleEditorBrushPreview()
	{
		mColor = Vector4(0, 0, 0, 0.45f);
	}
};