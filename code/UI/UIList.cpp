#include "UI/UIList.hpp"
#include "UI/UIManager.hpp"
#include "UI/UIBuilder.hpp"
#include "UI/UIText.hpp"
#include "Graphics/Graphics.hpp"
#include "Scenes/Transform.hpp"
#include "Scenes/Scene.hpp"

UIList::UIListEntry::UIListEntry(const std::string& label, UIElementCallback callback)
{
	mLabel = label;
	mCallback = callback;
}

void UIList::init()
{
	UIElement::init();

	setOnFocusLostCallback([this](UIElement *uiElement)
	{ 
		//setEntriesVisibility(false); 
	});

	subscribeToScrollEvents();
}

void UIList::initFromConfig(const UIElementConfig& config)
{
	UIElement::initFromConfig(config);

	getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	getTransform()->setScale(Vector3(UIUtils::correctAspectRatio_X(mConfig.mSize), 1));
	getTransform()->setAffectedByProjection(false);

	Renderer* renderer = NEW(Renderer);
	renderer->init();

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mConfig.mMaterial);
	renderer->setColor(mConfig.mStyle->mBackgroundColor);
	renderer->setDepth(mConfig.mLayer);
	//renderer->setHasBorder(true);

	//renderer->setClipRectangle(Rectangle(Vector2(mConfig.mPosition.x, mConfig.mPosition.y), Vector2(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y)));
	
	addComponent<Renderer>(renderer);
	
	setComponentsCache();

	setOnPressedCallback([self = this](UIElement* uiElement) 
	{
		self->toggle();
	});
}

void UIList::onDestroy()
{
	UIElement::onDestroy();
}

UIList &UIList::addOption(const std::string &label, UIElementCallback onPressedCallback)
{
	mEntries.push_back(UIListEntry(label, onPressedCallback));
	return *this;
}

void UIList::toggle()
{
	// TODO : Temporary
	if (mButtons.empty())
	{
		Vector3 scale = getTransform()->getScale();
		scale.x = scale.x * RenderContext::getAspectRatio();

		UIBuilder uiBuilder;

		uiBuilder.
			setLayout(UILayout::VERTICAL).
			//setSize(scale).
			setPosition(Vector2((-scale.x / 2.0f) / RenderContext::getAspectRatio(), scale.y/2.0f)).
			setTextSize(mConfig.mTextSize).
			setAdjustSizeToText(true).
			setLayer(mConfig.mLayer);

		FOR_LIST(it, mEntries)
		{
			std::string &label = (*it).mLabel;
			UIElementCallback onPressedCallback = (*it).mCallback;

			uiBuilder.
			setText(label).
			create<UIButton>();

			UIButton *button = (UIButton *)uiBuilder.getUIElement();
			button->setOnPressedCallback(onPressedCallback);
			button->setVisibility(false);

			Transform *t = button->getTransform();
			t->setParent(getTransform());

			Rectangle clipRectangle(
						Vector2(mConfig.mPosition.x, mConfig.mPosition.y),
						Vector2(mConfig.mSize.x / RenderContext::getAspectRatio(), mConfig.mSize.y));

			button->getRenderer()->setClipRectangle(clipRectangle);

			// Set clip rectangle for UIText label also
			FOR_LIST(itRenderer, *button->getText()->getComponents<Renderer>())
			{
				(*itRenderer)->setClipRectangle(clipRectangle);
			}

			mButtons.push_back(button);
		}

		//setEntriesVisibility(false);
	}

	FOR_LIST(it, mButtons)
	{
		(*it)->setVisibility(!(*it)->isVisible());
	}

	// TODO : If I want to create-remove buttons, I have to implement TIMER NEXT FRAME!
	//setEntriesVisibility(mButtons->isEmpty());
}

void UIList::setEntriesVisibility(bool visible)
{
	/*if(visible){
		FOR_LIST(it, mEntries) {

			std::string& label = it.get().mLabel;
			UIElementCallback onPressedCallback = it.get().mCallback;

			Vector3 scale = getTransform()->getScale();
			scale.x = scale.x * RenderContext::getAspectRatio();

			UIManager::getInstance().getBuilder()->saveData()->
				setPosition(Vector2(-scale.x/2.0f,-scale.y* mButtons->getLength() - scale.y/2.0f))->
				setSize(scale)->
				setText(label)->
				setAdjustSizeToText(true)->
				setLayer(getRenderer()->getLayer() + 1)->
				setIsAffectedByLayout(false)->
				create<UIButton>();

			UIButton* button = (UIButton*) UIManager::getInstance().getBuilder()->getUIElement();
			button->setOnPressedCallback(onPressedCallback);
			//button->setVisibility(false);

			Transform* t = button->getTransform();
			t->setParent(getTransform());

			UIManager::getInstance().getBuilder()->restoreData();

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

void UIList::onScroll(f32 scroll)
{
	UIElement::onScroll(scroll);

	if (isVisible())
	{
		if (isMouseCursorInsideElement())
		{
			FOR_LIST(it, mButtons)
			{
				(*it)->getTransform()->translate(Vector2(0,0.005f * -scroll));
			}
		}
	}
}