#include "UI/UIDropdown.hpp"
#include "UI/UIManager.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Graphics.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"

UIDropdown::UIDropdownEntry::UIDropdownEntry(const std::string& label, UIElementCallback callback)
{
	mLabel = label;
	mCallback = callback;
}

void UIDropdownButton::onPostReleased()
{
	UIButton::onPostReleased();
	mParentDropdown->setEntriesVisibility(false);
}

void UIDropdown::init()
{
	UIButton::init();
}

void UIDropdown::onDestroy()
{
	UIButton::onDestroy();
}

void UIDropdown::onPostReleased()
{
	UIButton::onPostReleased();
	toggle();
}

UIDropdown &UIDropdown::addOption(const std::string &label, UIElementCallback onPressedCallback)
{
	mEntries.push_back(UIDropdownEntry(label, onPressedCallback));
	return *this;
}

void UIDropdown::toggle()
{
	// TODO : If I want to create-remove buttons, I have to implement TIMER NEXT FRAME!
	setEntriesVisibility(mButtons.empty() ? true : !mButtons.front()->isActive());
}

void UIDropdown::setEntriesVisibility(bool visible)
{
	if(visible)
	{
		if (mButtons.empty())
		{
			Vector3 scale = getTransform()->getScale();
			scale.x = scale.x * RenderContext::getAspectRatio();
			
			UIBuilder uiBuilder;

			uiBuilder.
				setLayout(UILayout::VERTICAL).
				//setSize(scale).
				setPosition(getTransform()->getWorldPosition() + Vector2((-scale.x / 2.0f) / RenderContext::getAspectRatio(), -scale.y / 2.0f)).
				setTextSize(mConfig.mTextSize).
				setAdjustSizeToText(true).
				setLayer(mConfig.mLayer);

			FOR_LIST(it, mEntries)
			{
				std::string &label = (*it).mLabel;
				UIElementCallback onPressedCallback = (*it).mCallback;

				uiBuilder.
				setText(label).
				create<UIDropdownButton>();

				UIDropdownButton *button = uiBuilder.getUIElement<UIDropdownButton>();
				button->setOnPressedCallback(onPressedCallback);

				button->setParentDropdown(this);

				mButtons.push_back(button);
			}
		}
		
		FOR_LIST(it, mButtons)
		{
			(*it)->setVisibility(true);
		}
	}
	else
	{
		if(!mButtons.empty())
		{
			FOR_LIST(it, mButtons)
			{
				getScene()->removeGameObject(*it);
			}

			mButtons.clear();
		}
	}
}