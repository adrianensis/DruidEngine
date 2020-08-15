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
	DE_CLASS(UIGroup, DE_Class);

	std::string mName;
	bool mVisible;
	List<UIElement*>* mUIElements;

	void init();
};

class UI: public DE_Class, public Singleton<UI> {
private:

	List<UIElement*>* mUIElements;
	HashMap<std::string, UIGroup*>* mGroups;

	HashMap<c8, Vector2>* mCharMap;

	Vector2 mFontTilesCount;
	Vector2 mFontTileTextureSize;

	Material* mFontMaterial;

	void internalRemoveUIElement(const Iterator *it);

public:

	DE_CLASS(UI, DE_Class);

	UIBuilder* const getBuilder() const;

	void init();
	void addUIElement(UIElement* uiElement);
	void step();
	void terminate();

	// fonts

	Vector2 getCharTextureCoordinates(c8 character);
	Vector2 getFontTileTextureSize() const {
		return mFontTileTextureSize;
	};

	Material* getFontMaterial();

	// groups

	void addToGroup(const std::string& groupName, UIElement* uiElement);
	void removeFromGroup(const std::string& groupName, UIElement* uiElement);
	void removeElementsFromGroup(const std::string& groupName);
	void setGroupVisibility(const std::string& groupName, bool visibility);
};

} /* namespace DE */

#endif /* DE_UI_H */
