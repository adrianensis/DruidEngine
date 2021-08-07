#pragma once

#include "Core/ObjectBase.hpp"
#include "UI/UIElement.hpp"
#include <string>

class EditorController;

CLASS(ToolsBar, ObjectBase)
{
	PRI(EditorController, NONE, EditorController*);

private:
	void addButton(const std::string &name, UIElementCallback callback) const;

public:
	void init(EditorController* editorController);
};