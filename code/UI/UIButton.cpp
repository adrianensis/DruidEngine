#include "UI/UIButton.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Events/EventsManager.hpp"
#include "Graphics/Mesh.hpp"

void UIButton::init()
{
	Super::init();

	subscribeToMouseEvents();
}

void UIButton::initFromConfig(const UIElementConfig& config)
{
	Super::initFromConfig(config);

	getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	getTransform()->setScale(Vector3(UIUtils::correctAspectRatio_X(mConfig.mSize), 1));
	getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mConfig.mMaterial);
	renderer->setLayer(mConfig.mLayer);
	renderer->setColor(mConfig.mStyle->mColor);

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

	Super::onDestroy();
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
			mText = UI::getInstance()->getUIBuilder().
			saveData().
			setPosition(Vector2(0, 0)).
			setText(text).
			setLayer(mConfig.mLayer + 1).
			setIsAffectedByLayout(false).
			setParent(this).
			create<UIText>().
			getUIElement<UIText>();

			UI::getInstance()->getUIBuilder().restoreData();
		}

		mText->setText(text);
	}
}

void UIButton::setVisibility(bool visibility)
{
	Super::setVisibility(visibility);

	if (mText)
	{
		mText->setVisibility(visibility);
	}
}

void UIToggleButton::init()
{
	Super::init();

	mCanToggle = true;
	mReleaseOnSameGroupPressed = true;
}