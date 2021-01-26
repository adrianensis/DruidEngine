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
	mBackgroundColor3 = Vector4(0.2,0.2,0.2,1);
	mBackgroundColor4 = Vector4(0.5,0.5,0.5,0.7);
}

// ---------------------------------------------------------------------------


UIBuilder::UIBuilder() : DE_Class(), Singleton() {
	mScene = nullptr;
	mButtonMaterial = nullptr;
	mCurrentLayout = UILayout::VERTICAL;
	mMakeRelativeToLastData = false;
	mCurrentUIElement = nullptr;

	mData.init(Vector2(0,0), Vector2(0,0), "", 0);
	//mSavedData.init(Vector2(0,0), Vector2(0,0), "", 0);

	mDataStack = nullptr;
}

UIBuilder::~UIBuilder() {
	DE_FREE(mDataStack);
}

// ---------------------------------------------------------------------------

void UIBuilder::registerCurrentUIElement(UIElement* uiElement) {
	mScene->addGameObject(uiElement);

	mCurrentUIElement = uiElement;

	if(mData.mIsAffectedByLayout) {

		if(mNewRowOrColumn) {
			mNewRowOrColumn = false;
			mLayoutFirstUIElementData = mData;
		}

		mMakeRelativeToLastData = true;

		mLastData = mData;
	}
}

// ---------------------------------------------------------------------------

UILayout UIBuilder::getOppositeLayout(UILayout layout) {
	return (UILayout)(((int)mCurrentLayout + 1) % (int)UILayout::MAX);
}

Vector2 UIBuilder::calculateNextElementOffset(UILayout layout) {
	Vector2 offset = Vector2(0,0);

	switch (layout) {
		case UILayout::HORIZONTAL: {
			offset = Vector2(mLastData.mSize.x + mData.mSeparatorSize, 0);
			break;
		}
		case UILayout::VERTICAL: {
			offset = Vector2(0, -(mLastData.mSize.y + mData.mSeparatorSize));
			break;
		}
	}

	return offset;
}

void UIBuilder::calculateData() {

	if(mData.mAdjustSizeToText) {
		f32 offset = mData.mTextSize.x;
		mData.mSize.x = (mData.mTextSize.x * mData.mText.length()) /*+ offset*/;
		mData.mSize.y = mData.mTextSize.y;
	}

	if(mData.mIsAffectedByLayout && mMakeRelativeToLastData) {
		Vector2 offset = calculateNextElementOffset(mNewRowOrColumn ? getOppositeLayout(mCurrentLayout) : mCurrentLayout);
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
			break;
	}
}

// ---------------------------------------------------------------------------

UIBuilder* const UIBuilder::nextRow() {
	mLastData = mLayoutFirstUIElementData;
	mNewRowOrColumn = true;
	return this;
}

UIBuilder* const UIBuilder::nextColumn() {
	return nextRow(); // NOTE : exactly the same code.
}

// ---------------------------------------------------------------------------

UIBuilder* const UIBuilder::saveData() {
	if(!mDataStack){
		mDataStack = DE_NEW<List<UIElementData>>();
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

UIElement* UIBuilder::internalCreatePanel() {
	calculateData();

	UIElement* uiPanel = DE_NEW<UIButton>();
	uiPanel->init();
	uiPanel->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiPanel->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiPanel->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = DE_NEW<Renderer>();
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	//renderer->setMaterial(mButtonMaterial);
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor4);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	uiPanel->setComponentsCache();

	return uiPanel;
}

// ---------------------------------------------------------------------------

UIButton* UIBuilder::internalCreateButton() {

	calculateData();

	UIButton* uiButton = DE_NEW<UIButton>();
	uiButton->init();
	uiButton->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 size = mData.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	uiButton->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiButton->getTransform()->setScale(size);
	uiButton->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = DE_NEW<Renderer>();
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setLayer(mData.mLayer);
	renderer->setColor(mData.mBackgroundColor);
	//renderer->setHasBorder(true);

	RigidBody* rigidBody = DE_NEW<RigidBody>();
	uiButton->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = DE_NEW<Collider>();
	uiButton->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->getBoundingBox();

	uiButton->setComponentsCache();

	uiButton->setText(mData.mText);

	return uiButton;
}

// ---------------------------------------------------------------------------

UIText* UIBuilder::internalCreateText() {

	calculateData();

	UIText* uiText = DE_NEW<UIText>();
	uiText->init();
	uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 textSize = mData.mTextSize;
	textSize.z = 1;
	textSize.x = textSize.x / RenderContext::getAspectRatio();

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(mData.mTextSize.x / RenderContext::getAspectRatio(), mData.mTextSize.y, 1));
	uiText->getTransform()->setAffectedByProjection(false);

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	/*RigidBody* rigidBody = DE_NEW<RigidBody>();
	uiText->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = DE_NEW<Collider>();
	uiText->addComponent<Collider>(collider);
	collider->setSize(textSize.x, textSize.y);
	collider->getBoundingBox();*/

	uiText->setComponentsCache();

	return uiText;
}

// ---------------------------------------------------------------------------

UITextEditable* UIBuilder::internalCreateTextEditable() {

	calculateData();

	UITextEditable* uiText = DE_NEW<UITextEditable>();
	uiText->init();
	uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 size = mData.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	f32 halfSizeX = size.x/2.0f;

	Vector3 textSize = mData.mTextSize;
	textSize.z = 1;
	textSize.x = textSize.x / RenderContext::getAspectRatio();

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(textSize);
	uiText->getTransform()->setAffectedByProjection(false);

	RigidBody* rigidBody = DE_NEW<RigidBody>();
	uiText->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = DE_NEW<Collider>();
	uiText->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->setPositionOffset(Vector3(halfSizeX - (textSize.x),0,0));
	collider->getBoundingBox();

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	uiText->setComponentsCache();

	// Background
	f32 atlasBackgroundMargin = 0.1f;

	GameObject* background = DE_NEW<GameObject>();
	background->init();
	background->setIsStatic(true);

	background->setShouldPersist(false);

	background->getTransform()->setLocalPosition(aspectRatioCorrectedPosition + Vector3(halfSizeX - (textSize.x),0,0));
	background->getTransform()->setScale(size);
	background->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = DE_NEW<Renderer>();
	background->addComponent<Renderer>(renderer);

	renderer->setLayer(mData.mLayer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor3);
	//renderer->setHasBorder(true);

	mScene->addGameObject(background);

	return uiText;
}

// ---------------------------------------------------------------------------

UIDropdown* UIBuilder::internalCreateDropdown() {

	calculateData();

	UIDropdown* uiDropdown = DE_NEW<UIDropdown>();
	uiDropdown->init();
	uiDropdown->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiDropdown->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiDropdown->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiDropdown->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = DE_NEW<Renderer>();
	uiDropdown->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor2);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	RigidBody* rigidBody = DE_NEW<RigidBody>();
	uiDropdown->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = DE_NEW<Collider>();
	uiDropdown->addComponent<Collider>(collider);
	collider->setSize(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y);
	collider->getBoundingBox();

	uiDropdown->setComponentsCache();

	uiDropdown->setOnPressedCallback([self = uiDropdown](UIElement* uiElement) {
		self->toggle();
	});

	uiDropdown->setText(mData.mText);

	return uiDropdown;
}

// ---------------------------------------------------------------------------

UIBuilder* const UIBuilder::create(UIElementType type) {

	mData.mElementType = type;

	UIElement* newElement = nullptr;

	switch (type) {
		case UIElementType::PANEL:
			newElement = internalCreatePanel();
			break;
		case UIElementType::BUTTON:
			newElement = internalCreateButton();
			break;
		case UIElementType::TEXT:
			newElement = internalCreateText();
			break;
		case UIElementType::TEXTEDITABLE:
			newElement = internalCreateTextEditable();
			break;
		case UIElementType::DROPDOWN:
			newElement = internalCreateDropdown();
			break;
		default:
			break;
	}

	registerCurrentUIElement(newElement);

	return this;
}

UIElement* UIBuilder::getUIElement() {
	return mCurrentUIElement;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
