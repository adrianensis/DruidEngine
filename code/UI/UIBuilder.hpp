#pragma once

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"
#include "UI/UIDropdown.hpp"

#include <string>

namespace DE {

class Texture;
class Material;
class Scene;

template<class T> class List;
template<class K, class V> class HashMap;

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

class UIElementData: public DE_Class {
public:

	DE_CLASS_BODY(UIElementData)

	DE_M(ElementType, UIElementType)
	DE_M(Position, Vector2)
	DE_M(DisplayPosition, Vector2)
	DE_M(Size, Vector2)
	DE_M(TextSize, Vector2)
	DE_M(Text, String)
	DE_M(AdjustSizeToText, bool)
	DE_M(Layer, u32)
	DE_M(IsAffectedByLayout, bool)
	DE_M(Group, String)
	DE_M(SeparatorSize, f32)
	DE_M(BackgroundColor, Vector4)
	DE_M(BackgroundColor2, Vector4)
	DE_M(BackgroundColor3, Vector4)
	DE_M(BackgroundColor4, Vector4)

	void init(const Vector2 &position, const Vector2 &size, StringRef text, u32 layer);

	UIElementData& operator=(const UIElementData& otherData)
	{
		// check for self-assignment
		if(&otherData == this)
			return *this;

		mElementType = otherData.mElementType;
		mPosition = otherData.mPosition;
		mDisplayPosition = otherData.mDisplayPosition;
		mSize = otherData.mSize;
		mTextSize = otherData.mTextSize;
		if(!otherData.mText.empty()){mText = otherData.mText;} 
		mAdjustSizeToText = otherData.mAdjustSizeToText;
		mLayer = otherData.mLayer;
		mIsAffectedByLayout = otherData.mIsAffectedByLayout;
		if(!otherData.mGroup.empty()){mGroup = otherData.mGroup;} 
		mSeparatorSize = otherData.mSeparatorSize;
		mBackgroundColor = otherData.mBackgroundColor;
		mBackgroundColor2 = otherData.mBackgroundColor2;
		mBackgroundColor3 = otherData.mBackgroundColor3;
		mBackgroundColor4 = otherData.mBackgroundColor4;

		return *this;
	}
};

#define DE_UI_BUILDER_DATA_SETTER(Class, Name) UIBuilder* const set##Name(Class _##Name)\
 { mData.m##Name = _##Name; return this; }

class UIBuilder: public DE_Class, public Singleton<UIBuilder> {
private:

	DE_M(Scene, Scene*)
	DE_M(CurrentLayout, UILayout)
	DE_M(ButtonMaterial, Material*)
	DE_M(Data, UIElementData)
	DE_M(DataStack, List<UIElementData>*)
	DE_M(LastData, UIElementData)
	DE_M(MakeRelativeToLastData, bool) // used for layouts
	DE_M(LayoutFirstUIElementData, UIElementData)
	DE_M(NewRowOrColumn, bool)
	DE_M(CurrentUIElement, UIElement*)

	void registerCurrentUIElement(UIElement* uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateData();
	UIElement* internalCreatePanel();
	UIButton* internalCreateButton();
	UIText* internalCreateText();
	UITextEditable* internalCreateTextEditable();
	UIDropdown* internalCreateDropdown();

public:

	DE_CLASS_BODY(UIBuilder)

	UIBuilder* const setLayout(UILayout layout) {
		mCurrentLayout = layout;
		mMakeRelativeToLastData = false; // reset
		mNewRowOrColumn = true;
		return this;
	}

	UIBuilder* const nextRow();
	UIBuilder* const nextColumn();

	UIBuilder* const setScene(Scene *scene) { mScene = scene; return this; }
	UIBuilder* const setData(UIElementData data) { mData = data; return this; }

	DE_UI_BUILDER_DATA_SETTER(bool, IsAffectedByLayout)
	DE_UI_BUILDER_DATA_SETTER(const Vector2&, Position)
	DE_UI_BUILDER_DATA_SETTER(const Vector2&, Size)
	DE_UI_BUILDER_DATA_SETTER(u32, Layer)
	DE_UI_BUILDER_DATA_SETTER(StringRef, Text)
	DE_UI_BUILDER_DATA_SETTER(Vector2, TextSize)
	DE_UI_BUILDER_DATA_SETTER(bool, AdjustSizeToText)
	DE_UI_BUILDER_DATA_SETTER(String, Group)
	DE_UI_BUILDER_DATA_SETTER(const Vector4&, BackgroundColor)
	DE_UI_BUILDER_DATA_SETTER(f32, SeparatorSize)

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

}

