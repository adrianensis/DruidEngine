#include "UI/UIBuilder.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Material/Material.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Scene/Transform.hpp"
#include "Scene/ScenesManager.hpp"
#include "Input/Input.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "UI/UI.hpp"
#include "UI/UIStyle.hpp"

UIElementData::UIElementData()
{
	mIsAffectedByLayout = true;
}

void UIElementData::init(const Vector2 &position, const Vector2 &size, u32 layer, std::string text /*= std::string()*/)
{
	mElementType = UIElementType::PANEL;
	mPosition = position;
	mDisplayPosition = Vector2(0, 0);
	mSize = size;
	mText = text;
	mLayer = layer;
	mTextSize = UI::getDefaultFontSize(); // TODO : move to config?
	mAdjustSizeToText = false;
	mIsAffectedByLayout = true;
	mGroup = "";
	mParent = nullptr;
	mSeparatorSize = 0.01f;
}

UIBuilder::UIBuilder()
{
	mButtonMaterial = nullptr;
	mCurrentLayout = UILayout::VERTICAL;
	mMakeRelativeToLastData = false;
	mCurrentUIElement = nullptr;

	mData.init(Vector2(0, 0), Vector2(0, 0), 0);
	//mSavedData.init(Vector2(0,0), Vector2(0,0), "", 0);
}

void UIBuilder::registerUIElement(UIElement *uiElement)
{
	ScenesManager::getInstance()->getCurrentScene()->addGameObject(uiElement);

	if (mData.mGroup.length() > 0)
	{
		UI::getInstance()->addToGroup(mData.mGroup, uiElement);
	}
}

void UIBuilder::registerCurrentUIElement(UIElement *uiElement)
{
	registerUIElement(uiElement);

	mCurrentUIElement = uiElement;

	if (mData.mIsAffectedByLayout)
	{
		if (mNewRowOrColumn)
		{
			mNewRowOrColumn = false;
			mLayoutFirstUIElementData = mData;
		}

		mMakeRelativeToLastData = true;

		mLastData = mData;
	}
}

UILayout UIBuilder::getOppositeLayout(UILayout layout)
{
	return (UILayout)(((int)mCurrentLayout + 1) % (int)UILayout::MAX);
}

Vector2 UIBuilder::calculateNextElementOffset(UILayout layout)
{
	Vector2 offset = Vector2(0, 0);

	switch (layout)
	{
	case UILayout::HORIZONTAL:
	{
		offset = Vector2(mLastData.mSize.x + mData.mSeparatorSize, 0);
		break;
	}
	case UILayout::VERTICAL:
	{
		offset = Vector2(0, -(mLastData.mSize.y + mData.mSeparatorSize));
		break;
	}
	}

	return offset;
}

void UIBuilder::calculateData()
{
	if (mData.mAdjustSizeToText)
	{
		f32 offset = mData.mTextSize.x;
		mData.mSize.x = (mData.mTextSize.x * mData.mText.length()) /*+ offset*/;
		mData.mSize.y = mData.mTextSize.y;
	}

	if (mData.mIsAffectedByLayout && mMakeRelativeToLastData)
	{
		Vector2 offset = calculateNextElementOffset(mNewRowOrColumn ? getOppositeLayout(mCurrentLayout) : mCurrentLayout);
		mData.mPosition = mLastData.mPosition + offset;
	}

	// Offset the UI Element so its Top-Left corner is the origin.
	mData.mDisplayPosition = mData.mPosition;
	switch (mData.mElementType)
	{
	case UIElementType::TEXT:
	case UIElementType::TEXTEDITABLE:
		mData.mDisplayPosition.x += mData.mTextSize.x;
		mData.mDisplayPosition.y -= mData.mTextSize.y / 2.0f;
		break;
	default:
		mData.mDisplayPosition.x += mData.mSize.x / 2.0f;
		mData.mDisplayPosition.y -= mData.mSize.y / 2.0f;
		break;
	}
}

UIBuilder &UIBuilder::nextRow()
{
	mLastData = mLayoutFirstUIElementData;
	mNewRowOrColumn = true;
	return *this;
}

UIBuilder &UIBuilder::nextColumn()
{
	return nextRow(); // NOTE : exactly the same code.
}

UIBuilder &UIBuilder::saveData()
{
	mDataStack.push_front(mData);
	return *this;
}

UIBuilder &UIBuilder::restoreData()
{
	mData = mDataStack.front();
	mDataStack.pop_front();
	return *this;
}

UIPanel *UIBuilder::internalCreatePanel()
{
	calculateData();

	UIPanel *uiPanel = NEW(UIPanel);
	uiPanel->init();
	uiPanel->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiPanel->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiPanel->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	//renderer->setMaterial(mButtonMaterial);
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	uiPanel->setComponentsCache();

	return uiPanel;
}

UIButton *UIBuilder::internalCreateButton()
{
	calculateData();

	UIButton *uiButton = NEW(UIButton);
	uiButton->init();
	uiButton->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 size = mData.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	uiButton->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiButton->getTransform()->setScale(size);
	uiButton->getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setLayer(mData.mLayer);
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	//renderer->setHasBorder(true);

	/*RigidBody* rigidBody = NEW(RigidBody);
	uiButton->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = NEW(Collider);
	uiButton->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->getBoundingBox();*/

	uiButton->setComponentsCache();

	uiButton->setText(mData.mText);

	return uiButton;
}

UIText *UIBuilder::internalCreateText()
{
	calculateData();

	UIText *uiText = NEW(UIText);
	uiText->init();
	uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	Vector3 textSize = mData.mTextSize;
	textSize.z = 1;
	textSize.x = textSize.x / RenderContext::getAspectRatio();

	uiText->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiText->getTransform()->setScale(Vector3(textSize.x, textSize.y, 1));
	uiText->getTransform()->setAffectedByProjection(false);

	if (mData.mParent)
	{
		uiText->getTransform()->setParent(mData.mParent->getTransform());
		uiText->getTransform()->setLocalPosition(Vector2(-textSize.x * mData.mText.length() / 2.0f + textSize.x,0));
	}

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	// RigidBody* rigidBody = NEW(RigidBody);
	// uiText->addComponent<RigidBody>(rigidBody);
	// rigidBody->setSimulate(false);

	// Collider* collider = NEW(Collider);
	// uiText->addComponent<Collider>(collider);
	// collider->setSize(textSize.x, textSize.y);
	// collider->getBoundingBox();

	uiText->setComponentsCache();

	return uiText;
}

/*UITextEditable* UIBuilder::internalCreateTextEditable() {
	calculateData();

	UITextEditable* uiText = NEW(UITextEditable);
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

	RigidBody* rigidBody = NEW(RigidBody);
	uiText->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = NEW(Collider);
	uiText->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->setPositionOffset(Vector3(halfSizeX - (textSize.x),0,0));
	collider->getBoundingBox();

	uiText->setSize(mData.mTextSize);
	uiText->setLayer(mData.mLayer);
	uiText->setText(mData.mText);

	uiText->setComponentsCache();

	UIElement* uiPanel = NEW(UIButton);
	uiPanel->init();
	uiPanel->setIsStatic(true);

	aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiPanel->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiPanel->getTransform()->setScale(Vector3(size.x, size.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mData.mBackgroundColor4);
	renderer->setLayer(mData.mLayer);
	renderer->setPositionOffset(Vector3(halfSizeX - (textSize.x),0,0));

	uiPanel->setComponentsCache();

	registerUIElement(uiPanel);
		
	return uiText;
}*/

UIDropdown* UIBuilder::internalCreateDropdown() {
	calculateData();

	UIDropdown* uiDropdown = NEW(UIDropdown);
	uiDropdown->init();
	uiDropdown->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2(mData.mDisplayPosition.x / RenderContext::getAspectRatio(), mData.mDisplayPosition.y);

	uiDropdown->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiDropdown->getTransform()->setScale(Vector3(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y, 1));
	uiDropdown->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	uiDropdown->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	renderer->setLayer(mData.mLayer);
	//renderer->setHasBorder(true);

	/*RigidBody* rigidBody = NEW(RigidBody);
	uiDropdown->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = NEW(Collider);
	uiDropdown->addComponent<Collider>(collider);
	collider->setSize(mData.mSize.x / RenderContext::getAspectRatio(), mData.mSize.y);
	collider->getBoundingBox();*/

	uiDropdown->setComponentsCache();

	uiDropdown->setOnPressedCallback([self = uiDropdown](UIElement* uiElement) {
		self->toggle();
	});

	uiDropdown->setText(mData.mText);

	return uiDropdown;
}

UIBuilder &UIBuilder::create(UIElementType type)
{
	mData.mElementType = type;

	UIElement *newElement = nullptr;

	switch (type)
	{
	case UIElementType::PANEL:
		newElement = internalCreatePanel();
		break;
	case UIElementType::BUTTON:
		newElement = internalCreateButton();
		break;
	case UIElementType::TEXT:
		newElement = internalCreateText();
		break;
	/*case UIElementType::TEXTEDITABLE:
			newElement = internalCreateTextEditable();
			break;*/
	case UIElementType::DROPDOWN:
		newElement = internalCreateDropdown();
		break;
	default:
		break;
	}

	registerCurrentUIElement(newElement);

	return *this;
}

UIElement *UIBuilder::getUIElement()
{
	return mCurrentUIElement;
}