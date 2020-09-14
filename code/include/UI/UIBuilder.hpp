#ifndef DE_UIBUILDER_H
#define DE_UIBUILDER_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "UITextEditable.hpp"
#include "UIDropdown.hpp"
#include "UIList.hpp"

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

	DE_CLASS(UIElementData, DE_Class);

	Vector2 mPosition;
	Vector2 mSize;
	std::string mText;
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

		mPosition = otherData.mPosition;
		mSize = otherData.mSize;
		mText = otherData.mText;
		mLayer = otherData.mLayer;
		mIsAffectedByLayout = otherData.mIsAffectedByLayout;
		mBackgroundColor = otherData.mBackgroundColor;
		mBackgroundColor2 = otherData.mBackgroundColor2;
		mBackgroundColor3 = otherData.mBackgroundColor3;
		mBackgroundColor4 = otherData.mBackgroundColor4;

		return *this;
	}
};

class UIBuilder: public DE_Class, public Singleton<UIBuilder> {
private:

	Scene* mScene;
	UILayout mCurrentLayout;
	Material* mButtonMaterial;
	UIElementData mData;
	List<UIElementData>* mDataStack;
	UIElementData mLastData;
	//UIElementData mSavedData;
	UIElement* mLastUIElement; // used for layouts
	UIElement* mCurrentUIElement;

	void calculateData();
	UIElement* createPanel();
	UIButton* createButton();
	UIText* createText();
	UITextEditable* createTextEditable();
	UIDropdown* createDropdown();

public:

	DE_CLASS(UIBuilder, DE_Class);

	UIBuilder* const setScene(Scene* scene);
	UIBuilder* const setPosition(const Vector2& position);
	UIBuilder* const setSize(const Vector2& size);
	UIBuilder* const setLayer(u32 layer);
	UIBuilder* const setText(const std::string& text);
	UIBuilder* const setBackgroundColor(Vector4 backgroundColor);
	UIBuilder* const restoreColors();
	UIBuilder* const setLayout(UILayout layout);
	UIBuilder* const setIsAffectedByLayout(bool affectedByLayout);
	UIBuilder* const setSeparatorSize(f32 separatorSize);
	UIBuilder* const restoreSeparatorSize();
	UIBuilder* const setData(UIElementData initialData);
	UIBuilder* const create(UIElementType type);

	UIBuilder* const saveData();
	UIBuilder* const restoreData();

	UIElement* getUIElement();
};

} /* namespace DE */

#endif /* DE_UIBUILDER_H */
