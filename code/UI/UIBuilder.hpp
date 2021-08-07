#pragma once

#include "Core/ObjectBase.hpp"
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

#include <string>
#include <list>

class Material;
class Scene;

#define UI_BUILDER_CONFIG_SETTER(Name, ...)   \
	UIBuilder &set##Name(__VA_ARGS__ _##Name) \
	{                                              \
		mConfig.m##Name = _##Name;                   \
		return *this;                               \
	}

CLASS(UIBuilder, ObjectBase), SINGLETON(UIBuilder)
{
	PRI(CurrentLayout, NONE, UILayout)
	PRI(ButtonMaterial, NONE, Material *)
	PRI(Config, NONE, UIElementConfig)
	PRI(ConfigStack, NONE, std::list<UIElementConfig>);
	PRI(LastConfig, NONE, UIElementConfig)
	PRI(MakeRelativeToLastConfig, NONE, bool) // used for layouts
	PRI(LayoutFirstUIElementConfig, NONE, UIElementConfig)
	PRI(NewRowOrColumn, NONE, bool)
	PRI(CurrentUIElement, NONE, UIElement *)

private:
	void registerUIElement(UIElement * uiElement);
	void registerCurrentUIElement(UIElement * uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateConfig();
	UIPanel *internalCreatePanel();
	UIText *internalCreateText();
	UIButton* internalCreateButton();
	/*UITextEditable* internalCreateTextEditable();*/
	UIDropdown* internalCreateDropdown();
	UIDropdownButton* internalCreateDropdownButton();
	UIList* internalCreateList();

public:
	UIBuilder();

	UIBuilder &setLayout(UILayout layout)
	{
		mCurrentLayout = layout;
		mMakeRelativeToLastConfig = false; // reset
		mNewRowOrColumn = true;
		return *this;
	}

	UIBuilder &nextRow();
	UIBuilder &nextColumn();

	UIBuilder &setData(UIElementConfig data)
	{
		mConfig = data;
		return *this;
	}

	UI_BUILDER_CONFIG_SETTER(IsAffectedByLayout, bool)
	UI_BUILDER_CONFIG_SETTER(Position, const Vector2 &)
	UI_BUILDER_CONFIG_SETTER(Size, const Vector2 &)
	UI_BUILDER_CONFIG_SETTER(Layer, u32)
	UI_BUILDER_CONFIG_SETTER(Text, const std::string &)
	UI_BUILDER_CONFIG_SETTER(TextSize, Vector2)
	UI_BUILDER_CONFIG_SETTER(AdjustSizeToText, bool)
	UI_BUILDER_CONFIG_SETTER(Group, std::string)
	UI_BUILDER_CONFIG_SETTER(SeparatorSize, f32)
	UI_BUILDER_CONFIG_SETTER(Parent, GameObject*)
	UI_BUILDER_CONFIG_SETTER(Style, const UIStyle*)
	UI_BUILDER_CONFIG_SETTER(Material, Material*)

	UIBuilder &restoreSeparatorSize()
	{
		mConfig.mSeparatorSize = 0.01f;
		return *this;
	}

	UIBuilder &restoreStyle()
	{
		mConfig.mStyle = &UIStyleManager::getInstance()->getDefaultStyle();
		return *this;
	}

	UIBuilder &restoreMaterial();

	template<class T, typename = std::enable_if_t<std::is_base_of<UIElement, T>::value> >
	UIBuilder &create()
	{
		return create(T::getClassIdStatic());
	}

	UIBuilder &create(ClassId classId);

	UIBuilder &saveData();
	UIBuilder &restoreData();

	UIElement *getUIElement();

	template<class T, typename = std::enable_if_t<std::is_base_of<UIElement, T>::value> >
	T *getUIElement()
	{
		return dynamic_cast<T *>(mCurrentUIElement);
	}
};