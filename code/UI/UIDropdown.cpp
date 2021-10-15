#include "UI/UIDropdown.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"

UIDropdown::UIDropdownEntry::UIDropdownEntry(const SStr& label, UIElementCallback callback)
{
	mLabel = label;
	mCallback = callback;
}

void UIDropdownButton::onPostReleased()
{
	Super::onPostReleased();
	mParentDropdown->setEntriesVisibility(false);
}

void UIDropdown::init()
{
	Super::init();
}

void UIDropdown::onDestroy()
{
	Super::onDestroy();
}

void UIDropdown::onPostReleased()
{
	Super::onPostReleased();
	toggle();
}

UIDropdown &UIDropdown::addOption(const SStr &label, UIElementCallback onPressedCallback)
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
			
			UI::getInstance()->getUIBuilder().saveData().
				setLayout(UILayout::VERTICAL).
				//setSize(scale).
				setPosition(Vector2((-scale.x / 2.0f) / RenderContext::getAspectRatio(), -scale.y / 2.0f)).
				setTextSize(mConfig.mTextSize).
				setAdjustSizeToText(true).
				setLayer(mConfig.mLayer);

			FOR_LIST(it, mEntries)
			{
				SStr &label = (*it).mLabel;
				UIElementCallback onPressedCallback = (*it).mCallback;

				UI::getInstance()->getUIBuilder().
				setText(label).
				create<UIDropdownButton>();

				UIDropdownButton *button = UI::getInstance()->getUIBuilder().getUIElement<UIDropdownButton>();
				button->setOnPressedCallback(onPressedCallback);

				Transform *t = button->getTransform();
				t->setParent(getTransform());

				button->setParentDropdown(this);

				mButtons.push_back(button);
			}

			UI::getInstance()->getUIBuilder().restoreData();
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