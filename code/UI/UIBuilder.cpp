#include "UI/UIBuilder.hpp"
#include "Memory/Memory.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Material.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Renderer.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Shader.hpp"
#include "Scene/Transform.hpp"
#include "Input/Input.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "UI/UI.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIElementData::UIElementData() : DE_Class() {
	mIsAffectedByLayout = true;
}

UIElementData::~UIElementData() = default;

void UIElementData::init(const Vector2 &position, const Vector2 &size, const std::string& text, u32 layer){
	mElementType = UIElementType::PANEL;
	mPosition = position;
	mDisplayPosition = Vector2(0,0);
	mSize = size;
	mText = text;
	mLayer = layer;
	mTextSize = UI::getInstance()->getDefaultFontSize();
	mAdjustSizeToText = false;
	mIsAffectedByLayout = true;
	mSeparatorSize = 0.01f;
	mBackgroundColor = Vector4(0.5,0.5,0.5,1);
	mBackgroundColor2 = Vector4(0.6,0.6,0.6,1);
	mBackgroundColor3 = Vector4(0.4,0.4,0.4,1);
	mBackgroundColor4 = Vector4(0.5,0.5,0.5,0.7);
}

// ---------------------------------------------------------------------------


UIBuilder::UIBuilder() : DE_Class(), Singleton() {
	mScene = nullptr;
	mButtonMaterial = nullptr;
	mCurrentLayout = UILayout::VERTICAL;
	mLastUIElement = nullptr;
	mCurrentUIElement = nullptr;

	mData.init(Vector2(0,0), Vector2(0,0), "", 0);
	//mSavedData.init(Vector2(0,0), Vector2(0,0), "", 0);

	mDataStack = nullptr;
}

UIBuilder::~UIBuilder() {
	Memory::free<List<UIElementData>>(mDataStack);
}

// ---------------------------------------------------------------------------

void UIBuilder::calculateData(){

	if(mData.mAdjustSizeToText) {
		f32 textOffset = mData.mTextSize.x;
		mData.mSize.x = (mData.mTextSize.x * mData.mText.length()) + textOffset;
		mData.mSize.y = mData.mTextSize.y;
	}

	if(mData.mIsAffectedByLayout && mLastUIElement) {
		Vector2 offset = Vector2(0,0);

		switch (mCurrentLayout) {
			case UILayout::HORIZONTAL: {
				offset = Vector2(mLastData.mSize.x + mData.mSeparatorSize, 0);
				break;
			}
			case UILayout::VERTICAL:{
				offset = Vector2(0, -(mLastData.mSize.y + mData.mSeparatorSize));
				break;
			}
		}

		mData.mPosition = mLastData.mPosition + offset;
	}

	// Offset the UI Element so its Top-Left corner is the origin.
	mData.mDisplayPosition = mData.mPosition;
	switch (mData.mElementType) {
		case UIElementType::TEXT:
		case UIElementType::TEXTEDITABLE:
			mData.mDisplayPosition.x += mData.mTextSize.x/2.0f;
			mData.mDisplayPosition.y -= mData.mTextSize.y/2.0f;
			break;
		default:
			mData.mDisplayPosition.x += mData.mSize.x/2.0f;
			mData.mDisplayPosition.y -= mData.mSize.y/2.0f;
	}
}

UIBuilder* const UIBuilder::saveData() {
	if(!mDataStack){
		mDataStack = Memory::allocate<List<UIElementData>>();
		mDataStack->init();
	}

	mDataStack->pushFront(mData);
	//mSavedData.copy(mData);
	return this;
}

UIBuilder* const UIBuilder::restoreData() {
	//mData.copy(mSavedData);

	mData = mDataStack->popFront();
	return this;
}

// ---------------------------------------------------------------------------

UIElement* UIBuilder::createPanel() {
	calculateData();

	UIElement* uiPanel = Memory::allocate<UIButton>();
	uiPanel->init();
	uiPanel->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiPanel->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiPanel->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = Memory::allocate<Renderer>();
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	//renderer->setMaterial(mButtonMaterial);
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor4);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	uiPanel->setComponentsCache();


	mScene->addGameObject(uiPanel);

	mCurrentUIElement = uiPanel;

	if(mData.mIsAffectedByLayout)
		mLastUIElement = mCurrentUIElement;

	return uiPanel;
}

// ---------------------------------------------------------------------------

UIButton* UIBuilder::createButton() {

	calculateData();

	UIButton* uiButton = Memory::allocate<UIButton>();
	uiButton->init();
	uiButton->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 size = mData.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	uiButton->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiButton->getTransform()->setScale(size);
	uiButton->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = Memory::allocate<Renderer>();
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setLayer(mData.mLayer);
	renderer->setColor(mData.mBackgroundColor);
	//renderer->setHasBorder(true);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	uiButton->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = Memory::allocate<Collider>();
	uiButton->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->getBoundingBox();

	uiButton->setComponentsCache();

	mScene->addGameObject(uiButton);

	uiButton->setText(mData.mText);

	mCurrentUIElement = uiButton;

	if(mData.mIsAffectedByLayout)
		mLastUIElement = mCurrentUIElement;

	return uiButton;
}

// ---------------------------------------------------------------------------

UIText* UIBuilder::createText() {

	calculateData();

	UIText* uiText = Memory::allocate<UIText>();
	uiText->init();
	uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(mData.mTextSize.x / RenderContext::getAspectRatio(), mData.mTextSize.y, 1));
	uiText->getTransform()->setAffectedByProjection(false);

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	uiText->setComponentsCache();

	mScene->addGameObject(uiText);

	mCurrentUIElement = uiText;

	if(mData.mIsAffectedByLayout)
		mLastUIElement = mCurrentUIElement;

	return uiText;
}

// ---------------------------------------------------------------------------

UITextEditable* UIBuilder::createTextEditable() {

	calculateData();

	UITextEditable* uiText = Memory::allocate<UITextEditable>();
	uiText->init();
	uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(mData.mTextSize.x / RenderContext::getAspectRatio(), mData.mTextSize.y, 1));
	uiText->getTransform()->setAffectedByProjection(false);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	uiText->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	f32 halfSizeX = (mData.mTextSize.x * 4) / RenderContext::getAspectRatio();

	//f32 width = mData.mSize.x * mData.mText.length() / RenderContext::getAspectRatio();
	Collider* collider = Memory::allocate<Collider>();
	uiText->addComponent<Collider>(collider);
	collider->setSize(halfSizeX * 2.0f, mData.mTextSize.y);
	collider->setPositionOffset(Vector3(halfSizeX,0,0));
	collider->getBoundingBox();

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	uiText->setComponentsCache();

	// Background
	f32 atlasBackgroundMargin = 0.1f;

	GameObject* background = Memory::allocate<GameObject>();
	background->init();
	background->setIsStatic(true);

	background->setShouldPersist(false);

	background->getTransform()->setLocalPosition(aspectRatioCorrectedPosition + Vector3(halfSizeX,0,0));
	background->getTransform()->setScale(Vector3(halfSizeX * 2.2f, mData.mSize.y, 1));
	background->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = Memory::allocate<Renderer>();
	background->addComponent<Renderer>(renderer);

	renderer->setLayer(mData.mLayer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor3);
	//renderer->setHasBorder(true);

	mScene->addGameObject(background);

	mScene->addGameObject(uiText);

	mCurrentUIElement = uiText;

	if(mData.mIsAffectedByLayout)
		mLastUIElement = mCurrentUIElement;

	return uiText;
}

// ---------------------------------------------------------------------------

UIDropdown* UIBuilder::createDropdown() {

	calculateData();

	UIDropdown* uiDropdown = Memory::allocate<UIDropdown>();
	uiDropdown->init();
	uiDropdown->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiDropdown->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiDropdown->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiDropdown->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = Memory::allocate<Renderer>();
	uiDropdown->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor2);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	RigidBody* rigidBody = Memory::allocate<RigidBody>();
	uiDropdown->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = Memory::allocate<Collider>();
	uiDropdown->addComponent<Collider>(collider);
	collider->setSize(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y);
	collider->getBoundingBox();

	uiDropdown->setComponentsCache();

	uiDropdown->setOnPressedCallback([self = uiDropdown](UIElement* uiElement) {
		self->toggle();
	});

	mScene->addGameObject(uiDropdown);

	uiDropdown->setText(mData.mText);



	mCurrentUIElement = uiDropdown;

	if(mData.mIsAffectedByLayout)
		mLastUIElement = mCurrentUIElement;

	return uiDropdown;
}

// ---------------------------------------------------------------------------

UIBuilder* const UIBuilder::create(UIElementType type) {

	mData.mElementType = type;

	switch (type) {
		case UIElementType::PANEL:
			createPanel();
			break;
		case UIElementType::BUTTON:
			createButton();
			break;
		case UIElementType::TEXT:
			createText();
			break;
		case UIElementType::TEXTEDITABLE:
			createTextEditable();
			break;
		case UIElementType::DROPDOWN:
			createDropdown();
			break;
		default:
			break;
	}

	if(mData.mIsAffectedByLayout)
		mLastData = mData;

	return this;
}

UIElement* UIBuilder::getUIElement() {
	return mCurrentUIElement;
}

} /* namespace DE */
