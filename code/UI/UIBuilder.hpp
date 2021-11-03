#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIEditableText.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIText.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIList.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIStyle.hpp"
#include "Graphics/Material/MaterialManager.hpp"

class Material;
class Scene;

#define UI_BUILDER_CONFIG_SETTER(Name)   \
	UIBuilder &set##Name(SETTER_TYPE_FROM_VAR(mConfig.m##Name) new##Name) \
	{                                              \
		mConfig.m##Name = new##Name;                   \
		return *this;                               \
	}

#define UI_BUILDER_CONFIG_RESTORE(Name)   \
	UIBuilder &restore##Name() \
	{                                              \
		mConfig.m##Name = mDefaultConfig.m##Name;                   \
		return *this;                               \
	}

#define UI_BUILDER_CONFIG_METHODS(Name)\
	UI_BUILDER_CONFIG_RESTORE(Name)\
	UI_BUILDER_CONFIG_SETTER(Name)

class UIBuilder: public ObjectBase
{
    GENERATE_METADATA(UIBuilder)
	PRI UILayout mCurrentLayout = {};
	PRI UIElementConfig mConfig = {}; GETREF_CONST_SET(Config)
	PRI UIElementConfig mDefaultConfig = {};
	PRI std::list<UIElementConfig> mConfigStack = {};;
	PRI UIElementConfig mLastConfig = {};
	PRI bool mMakeRelativeToLastConfig = {}; // used for layouts
	PRI UIElementConfig mLayoutFirstUIElementConfig = {};
	PRI bool mNewRowOrColumn = {};
	PRI UIElement * mCurrentUIElement = {};

PRI
	void registerUIElement(UIElement * uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateConfig();

PUB
	UIBuilder();

	UI_BUILDER_CONFIG_METHODS(IsAffectedByLayout)
	UI_BUILDER_CONFIG_METHODS(Position)
	UI_BUILDER_CONFIG_METHODS(Size)
	UI_BUILDER_CONFIG_METHODS(Layer)
	UI_BUILDER_CONFIG_METHODS(Text)
	UI_BUILDER_CONFIG_METHODS(TextSize)
	UI_BUILDER_CONFIG_METHODS(AdjustSizeToText)
	UI_BUILDER_CONFIG_METHODS(Group)
	UI_BUILDER_CONFIG_METHODS(SeparatorSize)
	UI_BUILDER_CONFIG_METHODS(Parent)
	UI_BUILDER_CONFIG_METHODS(Style)
	UI_BUILDER_CONFIG_METHODS(Material)

	void restoreAll() { mConfig = mDefaultConfig; }

	UIBuilder &setLayout(UILayout layout)
	{
		mCurrentLayout = layout;
		mMakeRelativeToLastConfig = false; // reset
		mNewRowOrColumn = true;
		return *this;
	}

	UIBuilder &nextRow();
	UIBuilder &nextColumn();

	template<class T, typename = std::enable_if_t<std::is_base_of<UIElement, T>::value> >
	UIBuilder &create()
	{
		return create(T::getClassNameStatic());
	}

	UIBuilder &create(const std::string &className);

	UIBuilder &saveData();
	UIBuilder &restoreData();

	UIElement *getUIElement() const
	{
		return mCurrentUIElement;
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIElement, T>::value> >
	T *getUIElement() const
	{
		return dynamic_cast<T *>(getUIElement());
	}
};