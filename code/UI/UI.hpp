#pragma once

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UIList.hpp"
#include "UI/UIBuilder.hpp"

#include <string>

namespace DE {

class Texture;
class Material;
class Scene;
class Iterator;

template<class T> class List;
template<class K, class V> class HashMap;

class UIGroup: public DE_Class {
public:
	DE_CLASS_BODY(UIGroup)

	String mName;
	bool mVisible;
	List<UIElement*>* mUIElements;

	void init();
};

class UI: public DE_Class, public Singleton<UI> {
private:

	//List<UIElement*>* mUIElements;
	HashMap<String, UIGroup*>* mGroups;

	HashMap<c8, Vector2>* mCharMap;

	Vector2 mFontTilesCount;
	Vector2 mFontTileTextureSize;

	Material* mFontMaterial;

	Vector2 mDefaultFontSize = Vector2(0.04f / 2.0f, 0.04f);

	UIBuilder* mUIBuilder;

	UIElement* mFocusedElement = nullptr;

public:

	DE_CLASS_BODY(UI)

	UIBuilder* getBuilder();

	void init();
	void terminate();

	// fonts

	Vector2 getCharTextureCoordinates(c8 character);
	Vector2 getFontTileTextureSize() const { return mFontTileTextureSize; }
	Material* getFontMaterial();
	const Vector2& getDefaultFontSize() const { return mDefaultFontSize; }

	// groups

	void addToGroup(StringRef groupName, UIElement* uiElement);
	void removeFromGroup(StringRef groupName, UIElement* uiElement);
	void removeElementsFromGroup(StringRef groupName);
	void setGroupVisibility(StringRef groupName, bool visibility);

	// focus

	UIElement* getFocusedElement() const { return mFocusedElement; };
	void setFocusedElement(UIElement* focusedElement);
};

}

