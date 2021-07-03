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

UIBuilder::UIBuilder()
{
	mButtonMaterial = nullptr;
	mCurrentLayout = UILayout::VERTICAL;
	mMakeRelativeToLastConfig = false;
	mCurrentUIElement = nullptr;

	mConfig.init(Vector2(0, 0), Vector2(0, 0), 0);
	//mSavedData.init(Vector2(0,0), Vector2(0,0), "", 0);
}

void UIBuilder::registerUIElement(UIElement *uiElement)
{
	ScenesManager::getInstance()->getCurrentScene()->addGameObject(uiElement);

	if (mConfig.mGroup.length() > 0)
	{
		UI::getInstance()->addToGroup(mConfig.mGroup, uiElement);
	}
}

void UIBuilder::registerCurrentUIElement(UIElement *uiElement)
{
	uiElement->setConfig(mConfig);

	registerUIElement(uiElement);

	mCurrentUIElement = uiElement;

	if (mConfig.mIsAffectedByLayout)
	{
		if (mNewRowOrColumn)
		{
			mNewRowOrColumn = false;
			mLayoutFirstUIElementConfig = mConfig;
		}

		mMakeRelativeToLastConfig = true;

		mLastConfig = mConfig;
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
		offset = Vector2(mLastConfig.mSize.x + mConfig.mSeparatorSize, 0);
		break;
	}
	case UILayout::VERTICAL:
	{
		offset = Vector2(0, -(mLastConfig.mSize.y + mConfig.mSeparatorSize));
		break;
	}
	}

	return offset;
}

void UIBuilder::calculateConfig()
{
	if (mConfig.mAdjustSizeToText)
	{
		f32 offset = mConfig.mTextSize.x;
		mConfig.mSize.x = (mConfig.mTextSize.x * mConfig.mText.length()) /*+ offset*/;
		mConfig.mSize.y = mConfig.mTextSize.y;
	}

	if (mConfig.mIsAffectedByLayout && mMakeRelativeToLastConfig)
	{
		Vector2 offset = calculateNextElementOffset(mNewRowOrColumn ? getOppositeLayout(mCurrentLayout) : mCurrentLayout);
		mConfig.mPosition = mLastConfig.mPosition + offset;
	}

	// Offset the UI Element so its Top-Left corner is the origin.
	mConfig.mDisplayPosition = mConfig.mPosition;
	switch (mConfig.mElementType)
	{
	case UIElementType::TEXT:
	case UIElementType::TEXTEDITABLE:
		mConfig.mDisplayPosition.x += mConfig.mTextSize.x/RenderContext::getAspectRatio();
		mConfig.mDisplayPosition.y -= mConfig.mTextSize.y / 2.0f;
		break;
	default:
		mConfig.mDisplayPosition.x += (mConfig.mSize.x/RenderContext::getAspectRatio()) / 2.0f;
		mConfig.mDisplayPosition.y -= mConfig.mSize.y / 2.0f;
		break;
	}
}

UIBuilder &UIBuilder::nextRow()
{
	mLastConfig = mLayoutFirstUIElementConfig;
	mNewRowOrColumn = true;
	return *this;
}

UIBuilder &UIBuilder::nextColumn()
{
	return nextRow(); // NOTE : exactly the same code.
}

UIBuilder &UIBuilder::saveData()
{
	mConfigStack.push_front(mConfig);
	return *this;
}

UIBuilder &UIBuilder::restoreData()
{
	mConfig = mConfigStack.front();
	mConfigStack.pop_front();
	return *this;
}

UIPanel *UIBuilder::internalCreatePanel()
{
	calculateConfig();

	UIPanel *uiPanel = NEW(UIPanel);
	uiPanel->init();
	//uiPanel->setIsStatic(true);

	uiPanel->getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	uiPanel->getTransform()->setScale(Vector3(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	//renderer->setMaterial(mButtonMaterial);
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	renderer->setLayer(mConfig.mLayer);
	//renderer->setHasBorder(true);

	uiPanel->setComponentsCache();

	return uiPanel;
}

UIButton *UIBuilder::internalCreateButton()
{
	calculateConfig();

	UIButton *uiButton = NEW(UIButton);
	uiButton->init();
	//uiButton->setIsStatic(true);

	Vector3 size = mConfig.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	uiButton->getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	uiButton->getTransform()->setScale(size);
	uiButton->getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setLayer(mConfig.mLayer);
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

	uiButton->setText(mConfig.mText);

	return uiButton;
}

UIText *UIBuilder::internalCreateText()
{
	calculateConfig();

	UIText *uiText = NEW(UIText);
	uiText->init();
	//uiText->setIsStatic(true);

	Vector3 textSize = mConfig.mTextSize;
	textSize.z = 1;
	textSize.x = textSize.x / RenderContext::getAspectRatio();

	uiText->getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	uiText->getTransform()->setScale(Vector3(textSize.x, textSize.y, 1));
	uiText->getTransform()->setAffectedByProjection(false);

	if (mConfig.mParent)
	{
		uiText->getTransform()->setParent(mConfig.mParent->getTransform());
		uiText->getTransform()->setLocalPosition(Vector2(-textSize.x * mConfig.mText.length() / 2.0f + textSize.x,0));
	}

	uiText->setSize(mConfig.mTextSize);
	uiText->setLayer(mConfig.mLayer);
	uiText->setText(mConfig.mText);

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
	calculateConfig();

	UITextEditable* uiText = NEW(UITextEditable);
	uiText->init();
	//uiText->setIsStatic(true);

	Vector2 aspectRatioCorrectedPosition = Vector2((mConfig.mDisplayPosition.x / RenderContext::getAspectRatio()), mConfig.mDisplayPosition.y);

	Vector3 size = mConfig.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	f32 halfSizeX = size.x/2.0f;

	Vector3 textSize = mConfig.mTextSize;
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

	uiText->setSize(mConfig.mTextSize);
	uiText->setLayer(mConfig.mLayer);
	uiText->setText(mConfig.mText);

	uiText->setComponentsCache();

	UIElement* uiPanel = NEW(UIButton);
	uiPanel->init();
	uiPanel->setIsStatic(true);

	aspectRatioCorrectedPosition = Vector2((mConfig.mDisplayPosition.x / RenderContext::getAspectRatio()), mConfig.mDisplayPosition.y);

	uiPanel->getTransform()->setLocalPosition(aspectRatioCorrectedPosition);
	uiPanel->getTransform()->setScale(Vector3(size.x, size.y, 1));
	uiPanel->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	uiPanel->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(mConfig.mBackgroundColor4);
	renderer->setLayer(mConfig.mLayer);
	renderer->setPositionOffset(Vector3(halfSizeX - (textSize.x),0,0));

	uiPanel->setComponentsCache();

	registerUIElement(uiPanel);
		
	return uiText;
}*/

UIDropdown* UIBuilder::internalCreateDropdown() {
	calculateConfig();

	UIDropdown* uiDropdown = NEW(UIDropdown);
	uiDropdown->init();
	//uiDropdown->setIsStatic(true);

	uiDropdown->getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	uiDropdown->getTransform()->setScale(Vector3(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y, 1));
	uiDropdown->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	uiDropdown->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	renderer->setLayer(mConfig.mLayer);
	//renderer->setHasBorder(true);

	/*RigidBody* rigidBody = NEW(RigidBody);
	uiDropdown->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider* collider = NEW(Collider);
	uiDropdown->addComponent<Collider>(collider);
	collider->setSize(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y);
	collider->getBoundingBox();*/

	uiDropdown->setComponentsCache();

	uiDropdown->setOnPressedCallback([self = uiDropdown](UIElement* uiElement) {
		self->toggle();
	});

	uiDropdown->setText(mConfig.mText);

	return uiDropdown;
}

UIList* UIBuilder::internalCreateList()
{
	calculateConfig();

	UIList* uiList = NEW(UIList);
	uiList->init();
	//uiList->setIsStatic(true);

	uiList->getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	uiList->getTransform()->setScale(Vector3(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y, 1));
	uiList->getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	uiList->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(MaterialManager::getInstance()->loadNoTextureMaterial());
	renderer->setColor(UIStyleManager::getInstance()->getDefaultStyle().mColor);
	renderer->setLayer(mConfig.mLayer);
	//renderer->setHasBorder(true);

	uiList->setComponentsCache();

	uiList->setOnPressedCallback([self = uiList](UIElement* uiElement) {
		self->toggle();
	});

	//renderer->setClipRectangle(Rectangle(Vector2(mConfig.mPosition.x, mConfig.mPosition.y), Vector2(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y)));

	return uiList;
}

UIBuilder &UIBuilder::create(UIElementType type)
{
	mConfig.mElementType = type;

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
	case UIElementType::LIST:
		newElement = internalCreateList();
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