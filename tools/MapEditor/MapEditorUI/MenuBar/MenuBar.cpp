#include "MenuBar.hpp"

#include "Core/TimeUtils.hpp"

#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"

#include "Core/Settings.hpp"

#include "Core/TimerManager.hpp"

#include "Scene/GameObjectBuilder.hpp"
#include "MapEditor.hpp"
#include "../MapEditorUI.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

// ---------------------------------------------------------------------------

MenuBar::MenuBar(){

}

MenuBar::~MenuBar(){

}

// ---------------------------------------------------------------------------

void MenuBar::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	createMenuBar();
}

// ---------------------------------------------------------------------------

void MenuBar::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseY = 0.95f;

	EditorBuilder::getInstance()->createPanel(Vector2(-1 * RenderContext::getAspectRatio(),1), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-1 * RenderContext::getAspectRatio(), 1))->
		setSize(Vector2(0, 0.1f))->
		setAdjustSizeToText(true)->
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	EditorBuilder::getInstance()->createButton(mStringsUI.Save,[&, scene = scene](UIElement* uiElement) {
		scene->saveScene(scene->getPath());
	});

	EditorBuilder::getInstance()->createButton(mStringsUI.Collider,[&](UIElement* uiElement) {
		PhysicsEngine::getInstance()->setDebugColliders(!PhysicsEngine::getInstance()->getDebugColliders());
	});

	EditorBuilder::getInstance()->createButton(mStringsUI.Atlas,[&](UIElement* uiElement) {
		mMapEditor->mMapEditorUI.toggleAtlas();
	});

	EditorBuilder::getInstance()->createButton(mStringsUI.Play,[&](UIElement* uiElement) {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			mMapEditor->createPlayer();
		}

		mMapEditor->switchCameraControl();
	});

	EditorBuilder::getInstance()->createButton(mStringsUI.Grid,[&](UIElement* uiElement) {
		mMapEditor->mMapEditorUI.toggleGrid();
	});

	EditorBuilder::getInstance()->createButton(mStringsUI.AddSprite,[&](UIElement* uiElement) {
		mMapEditor->mMapEditorUI.createSpriteFromBrush();
	});

	UIDropdown* uiDropdown = (UIDropdown*) UI::getInstance()->getBuilder()->
		setText("Dropdown")->
		create(UIElementType::DROPDOWN)->
		getUIElement();

	uiDropdown->addOption("Option 1");
	uiDropdown->addOption("Option 2");
	uiDropdown->addOption("Option 3");

}

// ---------------------------------------------------------------------------

} /* namespace DE */
