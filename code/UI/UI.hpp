#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "Vector2.hpp"
#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "UIList.hpp"
#include "UIBuilder.hpp"

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
	DE_CLASS(UIGroup, DE_Class)

	std::string mName;
	bool mVisible;
	List<UIElement*>* mUIElements;

	void init();
};

class UI: public DE_Class, public Singleton<UI> {
private:

	//List<UIElement*>* mUIElements;
	HashMap<std::string, UIGroup*>* mGroups;

	HashMap<c8, Vector2>* mCharMap;

	Vector2 mFontTilesCount;
	Vector2 mFontTileTextureSize;

	Material* mFontMaterial;

	Vector2 mDefaultFontSize = Vector2(0.04f / 1.5f, 0.04f);

	void internalRemoveUIElement(const Iterator *it);

	UIBuilder* mUIBuilder;

	UIElement* mFocusedElement = nullptr;

public:

	DE_CLASS(UI, DE_Class)

	UIBuilder* getBuilder();

	void init();
	void terminate();

	// fonts

	Vector2 getCharTextureCoordinates(c8 character);
	Vector2 getFontTileTextureSize() const { return mFontTileTextureSize; }
	Material* getFontMaterial();
	const Vector2& getDefaultFontSize() const { return mDefaultFontSize; }

	// groups

	void addToGroup(const std::string& groupName, UIElement* uiElement);
	void removeFromGroup(const std::string& groupName, UIElement* uiElement);
	void removeElementsFromGroup(const std::string& groupName);
	void setGroupVisibility(const std::string& groupName, bool visibility);

	// focus

	UIElement* getFocusedElement() const { return mFocusedElement; };
	void setFocusedElement(UIElement* focusedElement) { mFocusedElement = focusedElement; };
};

} /* namespace DE */

#endif /* DE_UI_H */
