#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
/*#include "UI/UITextEditable.hpp"*/
#include "UI/UIDropdown.hpp"
#include "UI/UIText.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIList.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIStyle.hpp"
#include "Graphics/Material/MaterialManager.hpp"


class Material;
class Scene;

/*#define UI_BUILDER_CONFIG_SETTER(Name, ...)   \
	UIBuilder &set##Name(__VA_ARGS__ _##Name) \
	{                                              \
		mConfig.m##Name = _##Name;                   \
		return *this;                               \
	}*/

#define UI_BUILDER_CONFIG_SETTER(Name)   \
	UIBuilder &set##Name(SETTER_TYPE(mConfig.m##Name) _##Name) \
	{                                              \
		mConfig.m##Name = _##Name;                   \
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
	UI_BUILDER_CONFIG_SETTER(Name, ...)

CLASS(UIBuilder, ObjectBase), SINGLETON(UIBuilder)
{
	PRI(CurrentLayout, NONE, UILayout)
	PRI(ButtonMaterial, NONE, Material *)
	PRI(Config, NONE, UIElementConfig)
	PRI(DefaultConfig, NONE, UIElementConfig)
	PRI(ConfigStack, NONE, SLst<UIElementConfig>);
	PRI(LastConfig, NONE, UIElementConfig)
	PRI(MakeRelativeToLastConfig, NONE, bool) // used for layouts
	PRI(LayoutFirstUIElementConfig, NONE, UIElementConfig)
	PRI(NewRowOrColumn, NONE, bool)
	PRI(CurrentUIElement, NONE, UIElement *)

private:
	void registerUIElement(UIElement * uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateConfig();

public:
	UIBuilder();

	UI_BUILDER_CONFIG_SETTER(IsAffectedByLayout)
	UI_BUILDER_CONFIG_SETTER(Position)
	UI_BUILDER_CONFIG_SETTER(Size)
	UI_BUILDER_CONFIG_SETTER(Layer)
	UI_BUILDER_CONFIG_SETTER(Text)
	UI_BUILDER_CONFIG_SETTER(TextSize)
	UI_BUILDER_CONFIG_SETTER(AdjustSizeToText)
	UI_BUILDER_CONFIG_SETTER(Group)
	UI_BUILDER_CONFIG_SETTER(SeparatorSize)
	UI_BUILDER_CONFIG_SETTER(Parent)
	UI_BUILDER_CONFIG_SETTER(Style)
	UI_BUILDER_CONFIG_SETTER(Material)

	UI_BUILDER_CONFIG_RESTORE(SeparatorSize)
	UI_BUILDER_CONFIG_RESTORE(Style)
	UI_BUILDER_CONFIG_RESTORE(Material)

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

	UIBuilder &create(CNS SStr &className);

	UIBuilder &saveData();
	UIBuilder &restoreData();

	UIElement *getUIElement() CNS
	{
		return mCurrentUIElement;
	}

	template<class T, typename = std::enable_if_t<std::is_base_of<UIElement, T>::value> >
	T *getUIElement() CNS
	{
		return dynamic_cast<T *>(getUIElement());
	}
};