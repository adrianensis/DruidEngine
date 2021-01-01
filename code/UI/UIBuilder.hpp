#ifndef DE_UIBUILDER_H
#define DE_UIBUILDER_H

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"
#include "UI/UIDropdown.hpp"
#include "UI/UIList.hpp"

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
	VERTICAL
};

class UIElementData: public DE_Class {
public:

	DE_CLASS(UIElementData)

	UIElementType mElementType;
	Vector2 mPosition;
	Vector2 mDisplayPosition;
	Vector2 mSize;
	Vector2 mTextSize;
	std::string mText;
	bool mAdjustSizeToText;
	u32 mLayer;
	bool mIsAffectedByLayout;
	f32 mSeparatorSize;
	Vector4 mBackgroundColor;
	Vector4 mBackgroundColor2;
	Vector4 mBackgroundColor3;
	Vector4 mBackgroundColor4;

	void init(const Vector2 &position, const Vector2 &size, const std::string& text, u32 layer);

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
		mText = otherData.mText;
		mAdjustSizeToText = otherData.mAdjustSizeToText;
		mLayer = otherData.mLayer;
		mIsAffectedByLayout = otherData.mIsAffectedByLayout;
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

	Scene* mScene;
	UILayout mCurrentLayout;
	Material* mButtonMaterial;
	UIElementData mData;
	List<UIElementData>* mDataStack;
	UIElementData mLastData;
	UIElement* mLastUIElement; // used for layouts
	UIElement* mCurrentUIElement;

	void calculateData();
	UIElement* createPanel();
	UIButton* createButton();
	UIText* createText();
	UITextEditable* createTextEditable();
	UIDropdown* createDropdown();

public:

	DE_CLASS(UIBuilder)

	UIBuilder* const setLayout(UILayout layout) {
		mCurrentLayout = layout;

		// reset Last Element
		mLastUIElement = nullptr;

		return this;
	}

	UIBuilder* const setScene(Scene *scene) { mScene = scene; return this; }
	UIBuilder* const setData(UIElementData data) { mData = data; return this; }

	DE_UI_BUILDER_DATA_SETTER(bool, IsAffectedByLayout)
	DE_UI_BUILDER_DATA_SETTER(const Vector2&, Position)
	DE_UI_BUILDER_DATA_SETTER(const Vector2&, Size)
	DE_UI_BUILDER_DATA_SETTER(u32, Layer)
	DE_UI_BUILDER_DATA_SETTER(const std::string&, Text)
	DE_UI_BUILDER_DATA_SETTER(bool, AdjustSizeToText)
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

} /* namespace DE */

#endif /* DE_UIBUILDER_H */
