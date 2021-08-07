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

#define UI_BUILDER_CONFIG_SETTER(ObjectBase, Name)   \
	UIBuilder &set##Name(ObjectBase _##Name) \
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

	UI_BUILDER_CONFIG_SETTER(bool, IsAffectedByLayout)
	UI_BUILDER_CONFIG_SETTER(const Vector2 &, Position)
	UI_BUILDER_CONFIG_SETTER(const Vector2 &, Size)
	UI_BUILDER_CONFIG_SETTER(u32, Layer)
	UI_BUILDER_CONFIG_SETTER(const std::string &, Text)
	UI_BUILDER_CONFIG_SETTER(Vector2, TextSize)
	UI_BUILDER_CONFIG_SETTER(bool, AdjustSizeToText)
	UI_BUILDER_CONFIG_SETTER(std::string, Group)
	UI_BUILDER_CONFIG_SETTER(f32, SeparatorSize)
	UI_BUILDER_CONFIG_SETTER(GameObject*, Parent)
	UI_BUILDER_CONFIG_SETTER(const UIStyle*, Style)

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