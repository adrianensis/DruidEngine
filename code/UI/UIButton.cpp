#include "UI/UIButton.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Events/EventsManager.hpp"

void UIButton::init()
{
	Super::init();

	subscribeToMouseEvents();
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
			mText = (UIText *)UI::getInstance()->getUIBuilder().
			saveData().
			setPosition(Vector2(0, 0)).
			setText(text).
			setLayer(mConfig.mLayer + 1).
			setIsAffectedByLayout(false).
			setParent(this).
			create<UIText>().
			getUIElement();

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