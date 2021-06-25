#include "UI/UIDropdown.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"

UIDropdown::UIDropdownEntry::UIDropdownEntry(const std::string& label, UIElementCallback callback)
{
	mLabel = label;
	mCallback = callback;
}

void UIDropdown::init()
{
	UIButton::init();

	setOnFocusLostCallback([this](UIElement *uiElement)
	{ 
		//setEntriesVisibility(false); 
	});
}

void UIDropdown::onDestroy()
{
	UIButton::onDestroy();
}

UIDropdown &UIDropdown::addOption(const std::string &label, UIElementCallback onPressedCallback)
{
	mEntries.push_back(UIDropdownEntry(label, onPressedCallback));
	return *this;
}

void UIDropdown::toggle()
{
	// TODO : Temporary
	if (mButtons.empty())
	{
		Vector3 scale = getTransform()->getScale();
		scale.x = scale.x * RenderContext::getAspectRatio();

		Vector3 textScale = getText()->getTransform()->getScale();
		
		UI::getInstance()->getUIBuilder().saveData().
			setLayout(UILayout::VERTICAL).
			//setSize(scale).
			setPosition(Vector2(-scale.x / 2.0f, -scale.y / 2.0f)).
			setTextSize(Vector2(textScale.x * RenderContext::getAspectRatio() ,textScale.y)).
			setAdjustSizeToText(true).
			setLayer(getRenderer()->getLayer() + 1);

		FOR_LIST(it, mEntries)
		{
			std::string &label = (*it).mLabel;
			UIElementCallback onPressedCallback = (*it).mCallback;

			UI::getInstance()->getUIBuilder().
			setText(label).
			create(UIElementType::BUTTON);

			UIButton *button = (UIButton *)UI::getInstance()->getUIBuilder().getUIElement();
			button->setOnPressedCallback(onPressedCallback);
			button->setVisibility(false);

			Transform *t = button->getTransform();
			t->setParent(getTransform());

			

			mButtons.push_back(button);
		}

		UI::getInstance()->getUIBuilder().restoreData();

		//setEntriesVisibility(false);
	}

	FOR_LIST(it, mButtons)
	{
		(*it)->setVisibility(!(*it)->isVisible());
	}

	// TODO : If I want to create-remove buttons, I have to implement TIMER NEXT FRAME!
	//setEntriesVisibility(mButtons->isEmpty());
}

void UIDropdown::setEntriesVisibility(bool visible)
{
	/*if(visible){
		FOR_LIST(it, mEntries) {

			std::string& label = it.get().mLabel;
			UIElementCallback onPressedCallback = it.get().mCallback;

			Vector3 scale = getTransform()->getScale();
			scale.x = scale.x * RenderContext::getAspectRatio();

			UI::getInstance()->getBuilder()->saveData()->
				setPosition(Vector2(-scale.x/2.0f,-scale.y * mButtons->getLength() - scale.y/2.0f))->
				setSize(scale)->
				setText(label)->
				setAdjustSizeToText(true)->
				setLayer(getRenderer()->getLayer() + 1)->
				setIsAffectedByLayout(false)->
				create(UIElementType::BUTTON);

			UIButton* button = (UIButton*) UI::getInstance()->getBuilder()->getUIElement();
			button->setOnPressedCallback(onPressedCallback);
			//button->setVisibility(false);

			Transform* t = button->getTransform();
			t->setParent(getTransform());

			UI::getInstance()->getBuilder()->restoreData();

			mButtons->pushBack(button);
		}
	}
	else {
		if(!mButtons->isEmpty()){
			FOR_LIST(it, mButtons){
				getScene()->removeGameObject(it.get());
			}

			mButtons->clear();
		} 
	}*/

	FOR_LIST(it, mButtons)
	{
		(*it)->setVisibility(visible);
	}
}