#include "UI/UIDropdown.hpp"
#include "UI/UI.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Renderer.hpp"
#include "Scene/Transform.hpp"
#include "Containers/List.hpp"
#include "Scene/Scene.hpp"

namespace DE {

UIDropdown::UIDropdownEntry::UIDropdownEntry() : ObjectBase() {

}

UIDropdown::UIDropdownEntry::UIDropdownEntry(String label, UIElementCallback callback) : ObjectBase() {
	mLabel = label;
	mCallback = callback;
}

UIDropdown::UIDropdownEntry::~UIDropdownEntry() {

}

UIDropdown::UIDropdown() : UIButton() {
	mButtons = nullptr;
	mEntries = nullptr;
}

UIDropdown::~UIDropdown() {

}

void UIDropdown::init() {

	UIButton::init();

	mButtons = Memory::allocate<List<UIButton*>>();
	mButtons->init();

	mEntries = Memory::allocate<List<UIDropdownEntry>>();
	mEntries->init();

	setOnFocusLostCallback([this](UIElement* uiElement){		
		setEntriesVisibility(false);
	});
}

void UIDropdown::onDestroy() {
	Memory::free(mButtons);
	Memory::free(mEntries);
	UIButton::onDestroy();
}

UIDropdown* UIDropdown::addOption(StringRef label, UIElementCallback onPressedCallback) {

	mEntries->pushBack(UIDropdownEntry(label, onPressedCallback));

	return this;
}

void UIDropdown::toggle() {

	// TODO : Temporary
	if(mButtons->isEmpty()){
		FOR_LIST(it, mEntries) {

			StringRef label = it.get().mLabel;
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

		setEntriesVisibility(false);
	}

	FOR_LIST(it, mButtons){
		it.get()->setVisibility(!it.get()->isVisible());
	}

	// TODO : If I want to create-remove buttons, I have to implement TIMER NEXT FRAME!
	//setEntriesVisibility(mButtons->isEmpty());
}

void UIDropdown::setEntriesVisibility(bool visible){
	/*if(visible){
		FOR_LIST(it, mEntries) {

			StringRef label = it.get().mLabel;
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

	FOR_LIST(it, mButtons){
		it.get()->setVisibility(visible);
	}
} 

}
