#include "Editor.hpp"

#include "Core/Time/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
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
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"

#include "Command/CommandLine.hpp"

void Editor::init()
{
	
}

void Editor::firstStep()
{
	mEditorController.init();

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::HORIZONTAL).
	setPosition(Vector2(-1,1)).
	setLayer(0).
	setSize(Vector2(0.5f, 0.05f));

	UI::getInstance()->getUIBuilder().
	setAdjustSizeToText(true).
	setText("File").
	setTextSize(Vector2(0.025f, 0.05f)).
	create<UIDropdown>().
	getUIElement<UIDropdown>()->
	addOption("New", [](UIElement *uiElement)
	{

	}).
	addOption("Open", [&](UIElement *uiElement)
	{
		mEditorController.loadScene();
	}).
	addOption("Save", [&](UIElement *uiElement)
	{
		mEditorController.saveScene();
	});

	UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL).
	setPosition(Vector2(-1,0.8)).
	setLayer(0).
	setSize(Vector2(0.5f, 0.05f));

	UI::getInstance()->getUIBuilder().
	setText("Atlas").
	create<UIButton>().
	getUIElement<UIButton>()->
	setOnPressedCallback([&](UIElement* uiElement){
		mEditorController.getAtlas().toggle();
	});
}

void Editor::step()
{
	mEditorController.drawGrid();
}

void Editor::terminate()
{
	
}