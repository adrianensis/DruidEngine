#include "Editor.hpp"


#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Graphics.hpp"
#include "Input/Input.hpp"
#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"


#include "Command/CommandLine.hpp"

void Editor::init()
{
	
}

void Editor::firstUpdate()
{
	mEditorController.init();
}

void Editor::update()
{
	mEditorController.update();
}

void Editor::terminate()
{
	
}