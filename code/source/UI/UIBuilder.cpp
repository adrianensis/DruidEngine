#include "UIBuilder.hpp"
#include "Memory.hpp"
#include "Texture.hpp"
#include "MaterialManager.hpp"
#include "Material.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "RenderEngine.hpp"
#include "RenderContext.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "UI.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIElementData::UIElementData() : DE_Class() {

}

UIElementData::~UIElementData() = default;

void UIElementData::init(const Vector2 &position, const Vector2 &size, const std::string& text, u32 layer){
	mPosition = position;
	mSize = size;
	mText = text;
	mLayer = layer;
}

// ---------------------------------------------------------------------------

UIBuilder::UIBuilder() : DE_Class(), Singleton() {

	mButtonMaterial = nullptr;
}

UIBuilder::~UIBuilder() = default;

// ---------------------------------------------------------------------------

UIButton* UIBuilder::createButton(Scene *scene, const UIElementData& data) {

	if (!mButtonMaterial) {
		mButtonMaterial = MaterialManager::getInstance()->loadMaterial("resources/button.png");
	}

	UIButton* uiButton = Memory::allocate<UIButton>();
	uiButton->init();

	Vector2 aspectRatioCorrectedPosition = Vector2(data.mPosition.x / RenderContext::getAspectRatio(), data.mPosition.y);

	uiButton->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiButton->getTransform()->setScale(Vector3(data.mSize.x / RenderContext::getAspectRatio(), data.mSize.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mButtonMaterial);
	renderer->setLayer(data.mLayer);

	renderer->setAffectedByProjection(false);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	uiButton->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = Memory::allocate<Collider>();
	uiButton->addComponent<Collider>(collider);
	collider->setSize(data.mSize.x / RenderContext::getAspectRatio(), data.mSize.y);
	collider->getBoundingBox();

	uiButton->setComponentsCache();

	uiButton->setIsStatic(true);

	scene->addGameObject(uiButton);

	UI::getInstance()->addUIElement(uiButton);

	uiButton->setText(data.mText);

	return uiButton;
}

// ---------------------------------------------------------------------------

UIText* UIBuilder::createText(Scene *scene, const UIElementData& data) {



	UIText* uiText = Memory::allocate<UIText>();
	uiText->init();

	Vector2 aspectRatioCorrectedPosition = Vector2(data.mPosition.x / RenderContext::getAspectRatio(), data.mPosition.y);

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(data.mSize.x / RenderContext::getAspectRatio(), data.mSize.y, 1));

	uiText->setSize(data.mSize);
	uiText->setLayer(data.mLayer);
	uiText->setText(data.mText);

	uiText->setComponentsCache();

	uiText->setIsStatic(true);

	scene->addGameObject(uiText);

	UI::getInstance()->addUIElement(uiText);

	return uiText;
}

// ---------------------------------------------------------------------------

UIText* UIBuilder::createTextBox(Scene *scene, const UIElementData& data) {

	UIText* uiText = Memory::allocate<UIText>();
	uiText->init();

	Vector2 aspectRatioCorrectedPosition = Vector2(data.mPosition.x / RenderContext::getAspectRatio(), data.mPosition.y);

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(data.mSize.x / RenderContext::getAspectRatio(), data.mSize.y, 1));

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	uiText->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	f32 width = data.mSize.x * data.mText.length() / RenderContext::getAspectRatio();
	Collider* collider = Memory::allocate<Collider>();
	uiText->addComponent<Collider>(collider);
	collider->setSize(width, data.mSize.y);
	collider->setPositionOffset(Vector3(width/2.0f,0,0));
	collider->getBoundingBox();

	uiText->setSize(data.mSize);
	uiText->setLayer(data.mLayer);
	uiText->setText(data.mText);

	uiText->setComponentsCache();

	uiText->setIsStatic(true);

	uiText->setOnPressedCallback([self = uiText]() {
		Input::setInputCharReceiver(self);
		self->setText("");
	});

	scene->addGameObject(uiText);

	UI::getInstance()->addUIElement(uiText);

	return uiText;
}

// ---------------------------------------------------------------------------
//
//UIList* UIBuilder::createList(Scene *scene, const Vector2 &data.mPosition, const Vector2 &data.mSize, u32 data.mLayer) {
//
//	UIList* uiList = Memory::allocate<UIList>();
//	uiList->init();
//
//	uiList->getTransform()->setLocalPosition(data.mPosition);
//	uiList->getTransform()->setScale(Vector3(data.mSize.x, data.mSize.y, 1));
//
//	Renderer* renderer = Memory::allocate<Renderer>();
//	uiList->addComponent<Renderer>(renderer);
//
//	renderer->setMesh(Mesh::getRectangle());
//	renderer->setMaterial(mButtonMaterial);
//	renderer->setLayer(data.mLayer);
//
//	uiList->setComponentsCache();
//
//	uiList->setIsStatic(false);
//
//	f32 halfWidth = data.mSize.x / 2.0f;
//	f32 halfHeight = data.mSize.y / 2.0f;
//
//	f32 margin = 50;
//	f32 itemOffset = 50;
//
//	FOR_RANGE(i, 0, 3)
//	{
//		createText(scene, data.mPosition + Vector3(-halfWidth + margin, halfHeight - margin + -itemOffset * i, 0),
//				Vector2(40, 40), "list item", data.mLayer);
//	}
//
//	scene->addGameObject(uiList);
//
//	// scrollbar
//
//	f32 scrollbarMargin = 20;
//	UIButton* upButton = createButton(scene, data.mPosition + Vector3(halfWidth + scrollbarMargin, -halfHeight / 2.0f, 0),
//			Vector2(40, halfHeight - scrollbarMargin), data.mLayer);
//
//	upButton->setOnPressedCallback([&]() {
//
//	});
//
//	UIButton* downButton = createButton(scene, data.mPosition + Vector3(halfWidth + scrollbarMargin, halfHeight / 2.0f, 0),
//			Vector2(40, halfHeight - scrollbarMargin), data.mLayer);
//
//	downButton->setOnPressedCallback([&]() {
//
//	});
//
//	UI::getInstance()->addUIElement(uiList);
//
//	return uiList;
//}

// ---------------------------------------------------------------------------

UIElement* const UIBuilder::create(Scene *scene, UIElementData data, UIElementType type) {
	UIElement* newUIElement = nullptr;

	switch (type) {
		case UIElementType::BUTTON:
			newUIElement = createButton(scene, data);
			break;
		case UIElementType::TEXT:
			newUIElement = createText(scene, data);
			break;
		case UIElementType::TEXTBOX:
			newUIElement = createTextBox(scene, data);
			break;
		default:
			break;
	}

	return newUIElement;
}

} /* namespace DE */
