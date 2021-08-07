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

CLASS(UIStyleEditorToolsBar, UIStyleEditor)
{
public:
	UIStyleEditorToolsBar()
	{
		mColor = Vector4(0, 0, 0, 1);
		mColorSelected = Vector4(0.1f, 0.1f, 0.1f, 1);
		mColorHovered = Vector4(0.4f, 0.4f, 0.4f, 1);
	}
};