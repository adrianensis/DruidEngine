#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
/*#include "UI/UIButton.hpp"
#include "UI/UITextEditable.hpp"
#include "UI/UIDropdown.hpp"*/
#include "UI/UIText.hpp"
#include "UI/UIPanel.hpp"

#include <string>
#include <list>

class Material;
class Scene;

enum class UIElementType {
	PANEL,
	BUTTON,
	TEXT,
	TEXTEDITABLE,
	DROPDOWN
};

enum class UILayout {
	HORIZONTAL,
	VERTICAL,
	MAX
};

CLASS(UIElementData, ObjectBase) {

	PUB(ElementType, NONE, UIElementType)
	PUB(Position, NONE, Vector2)
	PUB(DisplayPosition, NONE, Vector2)
	PUB(Size, NONE, Vector2)
	PUB(TextSize, NONE, Vector2)
	PUB(Text, NONE, std::string)
	PUB(AdjustSizeToText, NONE, bool)
	PUB(Layer, NONE, u32)
	PUB(IsAffectedByLayout, NONE, bool)
	PUB(Group, NONE, std::string)
	PUB(SeparatorSize, NONE, f32)
	PUB(BackgroundColor, NONE, Vector4)
	PUB(BackgroundColor2, NONE, Vector4);
	PUB(BackgroundColor3, NONE, Vector4);
	PUB(BackgroundColor4, NONE, Vector4);

public:

	UIElementData();

	void init(const Vector2 &position, const Vector2 &size, u32 layer, std::string text = std::string());

	COPY(UIElementData) {
		DO_COPY(ElementType)
		DO_COPY(Position)
		DO_COPY(DisplayPosition)
		DO_COPY(Size)
		DO_COPY(TextSize)
		DO_COPY(AdjustSizeToText)
		DO_COPY(Layer)
		DO_COPY(IsAffectedByLayout)
		DO_COPY(SeparatorSize)
		DO_COPY(BackgroundColor)
		DO_COPY(BackgroundColor2)
		DO_COPY(BackgroundColor3)
		DO_COPY(BackgroundColor4)

		if(!other->mText.empty()){mText = other->mText;} 
		if(!other->mGroup.empty()){mGroup = other->mGroup;} 
	}
};

#define UI_BUILDER_DATA_SETTER(ObjectBase, Name)\
UIBuilder* const set##Name(ObjectBase _##Name)\
{ mData.m##Name = _##Name; return this; }

CLASS(UIBuilder, ObjectBase) , SINGLETON(UIBuilder) {

	PRI(CurrentLayout, NONE, UILayout)
	PRI(ButtonMaterial, NONE, Material*)
	PRI(Data, NONE, UIElementData)
	PRI(DataStack, NONE, std::list<UIElementData>);
	PRI(LastData, NONE, UIElementData)
	PRI(MakeRelativeToLastData, NONE, bool) // used for layouts
	PRI(LayoutFirstUIElementData, NONE, UIElementData)
	PRI(NewRowOrColumn, NONE, bool)
	PRI(CurrentUIElement, NONE, UIElement*)

private:

	void registerUIElement(UIElement* uiElement);
	void registerCurrentUIElement(UIElement* uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateData();
	UIPanel* internalCreatePanel();
	UIText* internalCreateText();
	/*UIButton* internalCreateButton();
	UITextEditable* internalCreateTextEditable();
	UIDropdown* internalCreateDropdown();*/

public:

	UIBuilder();

	UIBuilder* const setLayout(UILayout layout) {
		mCurrentLayout = layout;
		mMakeRelativeToLastData = false; // reset
		mNewRowOrColumn = true;
		return this;
	}

	UIBuilder* const nextRow();
	UIBuilder* const nextColumn();

	UIBuilder* const setData(UIElementData data) { mData = data; return this; }

	UI_BUILDER_DATA_SETTER(bool, IsAffectedByLayout)
	UI_BUILDER_DATA_SETTER(const Vector2&, Position)
	UI_BUILDER_DATA_SETTER(const Vector2&, Size)
	UI_BUILDER_DATA_SETTER(u32, Layer)
	UI_BUILDER_DATA_SETTER(const std::string&, Text)
	UI_BUILDER_DATA_SETTER(Vector2, TextSize)
	UI_BUILDER_DATA_SETTER(bool, AdjustSizeToText)
	UI_BUILDER_DATA_SETTER(std::string, Group)
	UI_BUILDER_DATA_SETTER(const Vector4&, BackgroundColor)
	UI_BUILDER_DATA_SETTER(f32, SeparatorSize)

	UIBuilder* const restoreColors() {
		mData.mBackgroundColor = Vector4(0.5,0.5,0.5,1);
		mData.mBackgroundColor2 = Vector4(0.6,0.6,0.6,1);
		mData.mBackgroundColor3 = Vector4(0.4,0.4,0.4,1);
		mData.mBackgroundColor4 = Vector4(0.5,0.5,0.5,0.7);
		return this;
	}

	UIBuilder* const restoreSeparatorSize() {
		mData.mSeparatorSize = 0.01f;
		return this;
	}

	UIBuilder* const create(UIElementType type);

	UIBuilder* const saveData();
	UIBuilder* const restoreData();

	UIElement* getUIElement();
};