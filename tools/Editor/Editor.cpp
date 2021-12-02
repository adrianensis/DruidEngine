#include "Editor.hpp"


#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Input/Input.hpp"
#include "UI/UI.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"

#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"


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