#include "UI/UIText.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Maths/Vector3.hpp"
#include "UI/UI.hpp"
#include "UI/UIPanel.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"
#include "Input/InputEvents.hpp"
#include "Events/Event.hpp"

#include "Graphics/RenderEngine.hpp"

UIText::UIText()
{
    mIsEditable = false;
}

void UIText::init()
{
	UIElement::init();
}

void UIText::initFromConfig(const UIElementConfig& config)
{
	UIElement::initFromConfig(config);

	Vector3 textSize = Vector3(UIUtils::correctAspectRatio_X(mConfig.mTextSize), 1);

	getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	getTransform()->setScale(textSize);
	getTransform()->setAffectedByProjection(false);

	if (mConfig.mParent)
	{
		getTransform()->setParent(mConfig.mParent->getTransform());
		getTransform()->setLocalPosition(Vector2(-textSize.x* mConfig.mText.length() / 2.0f + textSize.x,0));
	}

	setSize(mConfig.mTextSize);
	setLayer(mConfig.mLayer);

    setBackground(mConfig);

	setText(mConfig.mText);

	setComponentsCache();
}

void UIText::onDestroy()
{
	UIElement::onDestroy();
}

void UIText::setText(const std::string &text)
{
	if (mString != text)
	{
		bool newRenderersCreated = false;

		if (!mFontRenderers.empty())
		{
			if (text.empty())
			{
				FOR_ARRAY(i, mFontRenderers)
				{
					removeComponent(mFontRenderers[i]);
				}

				mFontRenderers.clear();
			}
			else
			{
				// Remove Extra Renderers
				if (mFontRenderers.size() > text.length())
				{
					FOR_RANGE(i, text.length(), mString.length())
					{
						removeComponent(mFontRenderers[i]);
					}

					std::vector<Renderer *> copyVector = mFontRenderers;
					mFontRenderers.clear();
					std::copy(copyVector.begin(), copyVector.begin() + text.length(), std::back_inserter(mFontRenderers));
				}
			}
		}

		if (!text.empty())
		{
			FOR_RANGE(i, 0, text.length())
			{
				Renderer *renderer = nullptr;

				char character = text.at(i);
				Vector2 textureCoordinates = UI::getInstance().getCharTextureCoordinates(character);
				Vector2 textureSize = UI::getInstance().getFontTileTextureSize();

				if (!mFontRenderers.empty() && i < static_cast<i32>(mString.length()))
				{
					renderer = mFontRenderers[i];
				}
				else
				{
					renderer = NEW(Renderer);
					renderer->init();

					renderer->setMesh(Mesh::getRectangle());
					renderer->setMaterial(UI::getInstance().getFontMaterial());
					renderer->setDepth(mLayer);
					
					addComponent<Renderer>(renderer);

					mFontRenderers.push_back(renderer);

					newRenderersCreated = true;
				}

				renderer->setPositionOffset(Vector3(((i* mSize.x) - (mSize.x / 2.0f)) / RenderContext::getAspectRatio(), 0, 0));
				renderer->setRegion(Rectangle(textureCoordinates.x, textureCoordinates.y, textureSize.x, textureSize.y));
			}
		}

		mString = text;

		// NOTE: IMPORTANT - REFRESH COMPONENTS
		setComponentsCache();
	}
}

void UIText::setIsEditable(bool editable)
{
    if(editable && !getIsEditable())
    {
        subscribeToMouseEvents();
        subscribeToEnterEvent();
        subscribeToEscEvent();
        subscribeToCharEvents();
        mOnlyReleaseOnClickOutside = true;
    }

    if(!editable && getIsEditable())
    {
        UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this);
        UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this);
        UNSUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this);
        UNSUBSCRIBE_TO_EVENT(InputEventKeyBackspace, nullptr, this);
        UNSUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this);
        UNSUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this);
        mOnlyReleaseOnClickOutside = true;
    }
}

void UIText::setVisibility(bool visibility)
{
    UIElement::setVisibility(visibility);

    if(mBackground)
    {
        mBackground->setVisibility(visibility);
    }
}
