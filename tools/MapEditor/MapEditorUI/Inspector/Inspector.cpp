#include "Inspector.hpp"

#include "Core/TimeUtils.hpp"

#include "Log/Log.hpp"

#include "Graphics/RenderEngine.hpp"

#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"

#include "Config/EngineConfig.hpp"

#include "Core/TimerManager.hpp"

#include "Scene/GameObjectBuilder.hpp"
#include "MapEditor.hpp"
#include "MapEditorUI/MapEditorUI.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

#include "MapElement/MapElement.hpp"

namespace DE {

Inspector::Inspector() : DE_Class(){

}

Inspector::~Inspector(){
	DE_FREE(mLayerButtons);
}

void Inspector::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;

	DE_SUBSCRIBE_TO_EVENT(EventOnSelectTile, &(mapEditor->mGrid), this,
	[&](const Event* event){
		updateInspectorOnSelectTile();
	});

	createInspector();
	createLayersBar();
}

void Inspector::setTileScaleFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex) {
	Transform* tileTransform = tile->getTransform();
	Vector3 scale = tileTransform->getScale();
	Vector3 newVector = Vector3(scale.x, scale.y, scale.z);
	newVector[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
	tileTransform->setScale(newVector);
	tile->getComponents<Renderer>()->get(0)->forceRecalculateVertices();
}

void Inspector::setTileColliderPositionFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex) {
	auto colliderList = tile->getComponents<Collider>();
	Collider* collider = colliderList ? colliderList->get(0) : nullptr;

	if(collider){
		Vector3 offset = collider->getPositionOffset();
		offset[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
		collider->setPositionOffset(offset);
	}
}

void Inspector::setTileColliderScaleFromInspector(GameObject* tile, StringRef stringValue, u32 vectorIndex) {
	auto colliderList = tile->getComponents<Collider>();
	Collider* collider = colliderList ? colliderList->get(0) : nullptr;

	if(collider) {
		Vector2 size = Vector2(collider->getWidth(), collider->getHeight());
		size[vectorIndex] = stringValue.length() > 0 ? std::stof(stringValue) : 0;
		collider->setSize(size.x, size.y);
	}
}

void Inspector::createInspector() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseX = 0.7f * RenderContext::getAspectRatio();
	f32 baseY = 0.8f;
	f32 separatorSize = 0.052f;

	Vector2 panelSize(0.5f, 1.8f);
	Vector2 panelPosition(1.0f * RenderContext::getAspectRatio() - panelSize.x, panelSize.y/2.0f);

	EditorBuilder::getInstance()->createPanel(panelPosition, panelSize);

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setAdjustSizeToText(true)->
		setPosition(Vector2(baseX, baseY))->
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	// Inspector
	EditorBuilder::getInstance()->createLabel(mStringsUI.Inspector);

	UI::getInstance()->getBuilder()->nextRow();

	// Tag

	mTextBoxTag = EditorBuilder::getInstance()->createTextBoxLabeled(mStringsUI.InspectorTileTag, "",
	[&](UIElement* uiElement) {
		mMapEditor->mGrid.forEachSelectedTile(
		[&](GameObject* tile){
			tile->setTag(uiElement->getInputString());
		});
	});

	// Position

	EditorBuilder::getInstance()->createLabel(mStringsUI.InspectorTilePosition);
	mTextInspectorX = EditorBuilder::getInstance()->createLabel("0.00");
	mTextInspectorY = EditorBuilder::getInstance()->createLabel("0.00");

	UI::getInstance()->getBuilder()->nextRow();

	// Size

	TextEditableVector2 textEditableVector2 = EditorBuilder::getInstance()->createTextBoxVector2(mStringsUI.InspectorSize,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile(
			[&](GameObject* tile) {
				setTileScaleFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile(
			[&](GameObject* tile) {
				setTileScaleFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxSizeX = textEditableVector2.TextEditableX;
	mTextBoxSizeY = textEditableVector2.TextEditableY;

	// Collider enable

	mButtonInspectorCollider = EditorBuilder::getInstance()->createRadialButton(mStringsUI.InspectorTileCollider,

	[&, self = mButtonInspectorCollider](UIElement* uiElement) {
		mMapEditor->mGrid.forEachSelectedTile(
		[&](GameObject* tile) {
			List<Collider*>* colliders = tile->getComponents<Collider>();

			if(colliders && !colliders->isEmpty()) {
				tile->removeComponent<Collider>(colliders->get(0));
				tile->removeComponent<RigidBody>(tile->getComponents<RigidBody>()->get(0));
				uiElement->setText(mStringsUI.BoolFalse);
			} else {
				mMapEditor->addColliderToTile(tile);
				uiElement->setText(mStringsUI.BoolTrue);
			}
		});
	});

	// Collider Position

	textEditableVector2 = EditorBuilder::getInstance()->createTextBoxVector2(mStringsUI.InspectorPosCollider,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderPositionFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderPositionFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxColliderPosX = textEditableVector2.TextEditableX;
	mTextBoxColliderPosY = textEditableVector2.TextEditableY;

	// Collider Size

	textEditableVector2 = EditorBuilder::getInstance()->createTextBoxVector2(mStringsUI.InspectorSizeCollider,
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderScaleFromInspector(tile, uiElement->getInputString(), 0);
			});
		},
		[&](UIElement* uiElement) {
			mMapEditor->mGrid.forEachSelectedTile([&](GameObject* tile){
				setTileColliderScaleFromInspector(tile, uiElement->getInputString(), 1);
			});
		}
	);

	mTextBoxColliderSizeX = textEditableVector2.TextEditableX;
	mTextBoxColliderSizeY = textEditableVector2.TextEditableY;
}

void Inspector::update() {

	MapElement* firstSelectedTile = mMapEditor->mGrid.getFirstSelectedTile();

	if(firstSelectedTile && firstSelectedTile->isActive()){
		Transform* tileTransform = firstSelectedTile->getTransform();

		mTextInspectorX->setText(std::to_string(tileTransform->getLocalPosition().x).substr(0,4));
		mTextInspectorY->setText(std::to_string(tileTransform->getLocalPosition().y).substr(0,4));

		List<Collider*>* colliders = firstSelectedTile->getComponents<Collider>();
		bool hasCollider = colliders && !colliders->isEmpty();

		mButtonInspectorCollider->setText(hasCollider ? mStringsUI.BoolTrue : mStringsUI.BoolFalse);
	}
}

void Inspector::updateInspectorOnSelectTile() {

	MapElement* firstSelectedTile = mMapEditor->mGrid.getFirstSelectedTile();

	if(firstSelectedTile && firstSelectedTile->isActive()){
		update();

		String tag = firstSelectedTile->getTag();
		mTextBoxTag->setText(/*tag.length() > 0 ?*/ tag /*: "none"*/);

		Transform* tileTransform = firstSelectedTile->getTransform();

		mTextBoxSizeX->setText(std::to_string(tileTransform->getScale().x).substr(0,4));
		mTextBoxSizeY->setText(std::to_string(tileTransform->getScale().y).substr(0,4));

		auto colliderList = firstSelectedTile->getComponents<Collider>();
		Collider* collider = colliderList && !colliderList->isEmpty() ? colliderList->get(0) : nullptr;

		mTextBoxColliderSizeX->setText(std::to_string(collider ? collider->getWidth() : 0.0f).substr(0,4));
		mTextBoxColliderSizeY->setText(std::to_string(collider ? collider->getHeight() : 0.0f).substr(0,4));

	}
}

void Inspector::createLayersBar() {

	u32 maxLayers = RenderEngine::getInstance()->getMaxLayers();
	mLayerButtons = DE_NEW<Array<UIButton*>>();
	mLayerButtons->init(maxLayers);

	Scene* scene = mMapEditor->getGameObject()->getScene();
	UIButton* button = nullptr;

	f32 separator = 0.08f;
	f32 baseX = 0.7f * RenderContext::getAspectRatio();
	f32 baseY = 0.0f;

	UI::getInstance()->getBuilder()->
		setAdjustSizeToText(true)->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(baseX, baseY))->
		restoreColors()->
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	FOR_RANGE(i, 0, maxLayers){
		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText(std::to_string(i))->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, Inspector = this](UIElement* uiElement) {
			Inspector->selectLayer(i);
		});

		mLayerButtons->set(i, button);

		button = (UIButton*) UI::getInstance()->getBuilder()->
		setText("on")->
		create(UIElementType::BUTTON)->
		getUIElement();

		button->setOnPressedCallback([&, i=i, self = button](UIElement* uiElement) {
			bool visibility = RenderEngine::getInstance()->getLayersData()->get(i)->mVisible;
			RenderEngine::getInstance()->getLayersData()->get(i)->mVisible = !visibility;
			self->setText(RenderEngine::getInstance()->getLayersData()->get(i)->mVisible ? "on" : "off");
		});

		UI::getInstance()->getBuilder()->nextRow();
	}

	selectLayer(0);
}

void Inspector::selectLayer(u32 layer){
	u32 maxLayers = RenderEngine::getInstance()->getMaxLayers();

	FOR_ARRAY(i, mLayerButtons){
		mLayerButtons->get(i)->getRenderer()->setColor(Vector4(0.6f,0.6f,0.6f,1));
	}

	mLayerButtons->get(layer)->getRenderer()->setColor(Vector4(-1.0f,0,0.5f,1));

	mMapEditor->mLayer = layer;
}

}
