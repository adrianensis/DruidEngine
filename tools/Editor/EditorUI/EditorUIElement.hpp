#pragma once

#include "Core/Core.hpp"

class EditorController;

class EditorUIElement: public ObjectBase
{
    GENERATE_METADATA(EditorUIElement)
	PRI EditorController* mEditorController = nullptr; GET(EditorController);
	PRI bool mIsVisible = false; GET(IsVisible)

PUB
	EditorUIElement()
	{
		mIsVisible = true;
	}

	virtual void init(EditorController* editorController) { mEditorController = editorController; };
    
	virtual void toggle()
	{ 
		setVisibility(!mIsVisible);
	}

	virtual void setVisibility(bool visible) { mIsVisible = visible; };
};