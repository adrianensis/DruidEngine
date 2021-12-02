#include "UI/UIButton.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Graphics.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"


void UIButton::init()
{
	UIElement::init();

	subscribeToMouseEvents();
}

void UIButton::initFromConfig(const UIElementConfig& config)
{
	UIElement::initFromConfig(config);

	getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	getTransform()->setScale(Vector3(UIUtils::correctAspectRatio_X(mConfig.mSize), 1));
	getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	renderer->init();

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mConfig.mMaterial);
	renderer->setDepth(mConfig.mLayer);
	renderer->setColor(mConfig.mStyle->mBackgroundColor);

	addComponent<Renderer>(renderer);
	
	setComponentsCache();

	setText(mConfig.mText);
}

void UIButton::onDestroy()
{
	if (mText)
	{
		getScene()->removeGameObject(mText);
		mText = nullptr;
	}

	UIElement::onDestroy();
}

void UIButton::onLabelDestroy()
{
	mText = nullptr;
}

void UIButton::setText(const std::string &text)
{
	if (text.length() > 0)
	{
		Vector3 buttonScale = getTransform()->getScale();

		if (!mText)
		{
			UIBuilder uiBuilder;

			mText = uiBuilder.
			setPosition(Vector2(0, 0)).
			setText(text).
			setLayer(mConfig.mLayer - 1).
			setIsAffectedByLayout(false).
			setParent(this).
			create<UIText>().
			getUIElement<UIText>();
		}

		mText->setText(text);
	}
}

void UIButton::setVisibility(bool visibility)
{
	UIElement::setVisibility(visibility);

	if (mText)
	{
		mText->setVisibility(visibility);
	}
}

void UIToggleButton::init()
{
	UIButton::init();

	mCanToggle = true;
	mReleaseOnSameGroupPressed = true;
}