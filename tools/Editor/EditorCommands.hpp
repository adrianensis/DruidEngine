#pragma once

#include "Core/Core.hpp"

class EditorController;

class EditorCommands
{
public:
    static void registerEditorCommands(EditorController* editorController);
};