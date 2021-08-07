#include "UI/UIElement.hpp"

#include "Graphics/Renderer.hpp"

#include "Events/EventsManager.hpp"
#include "Input/InputEvents.hpp"
#include "Events/Event.hpp"

#include "Input/Input.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Geometry.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "UI/UI.hpp"
#include "UI/UIStyle.hpp"

void UIElement::init()
{
	GameObject::init();

	mPressed = false;
	mConsumeInput = true;
}

void UIElement::onDestroy()
{
	GameObject::onDestroy();
	UNSUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventScroll, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this);
	UNSUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this);

	if (hasFocus())
	{
		UI::getInstance()->setFocusedElement(nullptr);
	}
}

void UIElement::subscribeToKeyEvents()
{
	SUBSCRIBE_TO_EVENT(InputEventKeyPressed, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
		}
	});

	SUBSCRIBE_TO_EVENT(InputEventKeyReleased, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
		}
	});
}

void UIElement::subscribeToCharEvents()
{
	SUBSCRIBE_TO_EVENT(InputEventChar, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			// TODO : boolean to enable or disable : can receive char input?
			onChar(((const InputEventChar *)event)->mChar);
		}
	});
}

void UIElement::subscribeToMouseEvents()
{
	SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			const InputEventMouseButtonPressed *e = (const InputEventMouseButtonPressed *)event;

			if (e->mButton == GLFW_MOUSE_BUTTON_LEFT)
			{
				onPressed();
			}
		}
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			const InputEventMouseButtonReleased *e = (const InputEventMouseButtonReleased *)event;
			if (e->mButton == GLFW_MOUSE_BUTTON_LEFT)
			{
				onReleased();
			}
		}
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			//const InputEventMouseMoved *e = (const InputEventMouseMoved *)event;
			onMouseOver();
		}
	});
}

void UIElement::subscribeToScrollEvents()
{
	SUBSCRIBE_TO_EVENT(InputEventScroll, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			const InputEventScroll *e = (const InputEventScroll *)event;
			onScroll(e->mScroll);
		}
	});
}

void UIElement::subscribeToEnterEvent()
{
	SUBSCRIBE_TO_EVENT(InputEventKeyEnter, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			onFocusLost(); // TODO : call something more generic
		}
	});
}

void UIElement::subscribeToEscEvent()
{
	SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [this](const Event *event)
	{
		if (isActive())
		{
			onFocusLost(); // TODO : call something more generic
		}
	});
}

bool UIElement::hasFocus() const
{
	return this == UI::getInstance()->getFocusedElement();
}

void UIElement::onChar(char character)
{
	if (hasFocus())
	{
		mInputString += character;
		setText(mInputString);
	}
}

void UIElement::onFocusLost()
{
	if (hasFocus())
	{
		UI::getInstance()->setFocusedElement(nullptr);
		mOnFocusLostFunctor.execute();
	}
}

void UIElement::onFocus()
{
}

void UIElement::onPressed()
{
	//Collider* collider = getCollider();

	if (mRenderer->isActive())
	{
		if (isMouseCursorInsideElement())
		{
			mRenderer->setColor(mConfig.mStyle->mColorSelected);
			
			mPressed = true;

			if (!hasFocus())
			{
				UIElement *lastFocusedElement = UI::getInstance()->getFocusedElement();

				if (lastFocusedElement && lastFocusedElement->isActive())
				{
					lastFocusedElement->onFocusLost();
				}

				UI::getInstance()->setFocusedElement(this);
				onFocus();
			}

			if (getConsumeInput())
			{
				Input::getInstance()->clearMouseButton();
			}
		}
	}
}

void UIElement::onReleased()
{
	if(mPressed)
	{
		if (mRenderer->isActive())
		{
			if (hasFocus())
			{
				bool cursorInside = isMouseCursorInsideElement();

				if (cursorInside)
				{
					mOnPressedFunctor.execute();
				}

				mPressed = false;

				mOnReleasedFunctor.execute();

				if (getConsumeInput())
				{
					Input::getInstance()->clearMouseButton();
				}

				if(cursorInside)
				{
					mRenderer->setColor(mConfig.mStyle->mColorHovered);
				}
				else
				{
					mRenderer->setColor(mConfig.mStyle->mColor);
				}

				onFocusLost();
			}
		}
	}
}

void UIElement::onMouseOver()
{
	if(!mPressed)
	{
		if (mRenderer->isActive())
		{
			if (isMouseCursorInsideElement())
			{
				mRenderer->setColor(mConfig.mStyle->mColorHovered);
			}
			else
			{
				mRenderer->setColor(mConfig.mStyle->mColor);
			}
		}
	}
}

void UIElement::onScroll(f32 scroll)
{
	if (mRenderer->isActive())
	{
		if (isMouseCursorInsideElement())
		{
			mOnScrollFunctor.execute();
		}
	}
}

void UIElement::setComponentsCache()
{
	mRenderer = getFirstComponent<Renderer>();
	//mCollider = getFirstComponent<Collider>();
}

void UIElement::setVisibility(bool visibility)
{
	setIsActive(visibility);
}

bool UIElement::isVisible()
{
	return isActive();
}

bool UIElement::isMouseCursorInsideElement()
{
	Vector2 screenMousePosition(Input::getInstance()->getMousePosition());
	
	//collider->getBoundingBox(true); // force regenerate bounding box
	Vector2 mousePosition = screenMousePosition;

	if(getTransform()->getAffectedByProjection())
	{
		Vector2 worldMousePosition = Vector2(
			RenderEngine::getInstance()->getCamera()->screenToWorld(screenMousePosition));

		mousePosition = worldMousePosition;
	}

	return Geometry::testRectanglePoint(
		mRenderer->getVertices()[0],
		getTransform()->getScale().x,
		getTransform()->getScale().y,
		mousePosition, 0);
}