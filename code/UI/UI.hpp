#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIBuilder.hpp"

#include <string>
#include <map>
#include <list>

class Material;
class Scene;

CLASS(UIGroup, ObjectBase) {	
	PUB(Name, NONE, std::string)
	PUB(Visible, NONE, bool)
	PUB(UIElements, NONE, std::list<UIElement*>);

public:
	void init();
};

CLASS(UI, ObjectBase), SINGLETON(UI) {

	PRI(Groups, NONE, std::map<std::string, UIGroup*>)
	PRI(CharMap, NONE, std::map<char, Vector2>)
	PRI(FontTilesCount, NONE, Vector2)
	PRI(FontTileTextureSize, NONE, Vector2)
	PRI(FontMaterial, NONE, Material*)

	static Vector2 smDefaultFontSize;

	PRI(UIBuilder, GETREF, UIBuilder)

	PRI(FocusedElement, NONE, UIElement*);

public:
	
	void init();
	void terminate();

	// fonts

	const Vector2& getCharTextureCoordinates(char character);
	const Vector2& getFontTileTextureSize() const { return mFontTileTextureSize; }
	Material* getFontMaterial();
	static const Vector2& getDefaultFontSize() { return smDefaultFontSize; }

	// groups

	void addToGroup(std::string& groupName, UIElement* uiElement);
	void removeFromGroup(std::string& groupName, UIElement* uiElement);
	void removeElementsFromGroup(std::string& groupName);
	void setGroupVisibility(std::string& groupName, bool visibility);

	// focus

	UIElement* getFocusedElement() const { return mFocusedElement; };
	void setFocusedElement(UIElement* focusedElement);
};