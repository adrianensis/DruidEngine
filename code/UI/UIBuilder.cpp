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
	mIsAffectedByLayout = true;
}

UIElementData::~UIElementData() = default;

void UIElementData::init(const Vector2 &position, const Vector2 &size, const std::string& text, u32 layer){
	mPosition = position;
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
		f32 offset = mData.mTextSize.x;
		mData.mSize.x = (mData.mTextSize.x * mData.mText.length()) + offset;
		mData.mSize.y = mData.mTextSize.y;
	}

	if(mData.mIsAffectedByLayout && mLastUIElement) {
		Vector2 offset = Vector2(0,0);

		switch (mCurrentLayout) {
			case UILayout::HORIZONTAL: {
				f32 lastElementHalfSize = mLastData.mSize.x / 2.0f;
				f32 newElementHalfSize = mData.mSize.x / 2.0f;
				offset = Vector2(lastElementHalfSize + newElementHalfSize + mData.mSeparatorSize, 0);
				break;
			}
			case UILayout::VERTICAL:{
				f32 lastElementHalfSize = mLastData.mSize.y / 2.0f;
				f32 newElementHalfSize = mData.mSize.y / 2.0f;
				offset = Vector2(0, -(lastElementHalfSize + newElementHalfSize + mData.mSeparatorSize));
				break;
			}
		}

		mData.mPosition = mLastData.mPosition + offset;
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

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mPosition.x / RenderContext::getAspectRatio(), mData.mPosition.y);

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

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mPosition.x / RenderContext::getAspectRatio(), mData.mPosition.y);

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

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mPosition.x / RenderContext::getAspectRatio(), mData.mPosition.y);

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

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mPosition.x / RenderContext::getAspectRatio(), mData.mPosition.y);

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

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mPosition.x / RenderContext::getAspectRatio(), mData.mPosition.y);

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

	uiDropdown->setOnPressedCallback([self = uiDropdown]() {
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
