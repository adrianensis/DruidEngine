#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIBuilder.hpp"


class Material;
class Scene;
class UIGroup;

CLASS(UI, ObjectBase), SINGLETON(UI)
{
	PRI(Groups, NONE, std::map<std::string, UIGroup *>)
	PRI(CharMap, NONE, std::map<char, Vector2>)
	PRI(FontTilesCount, NONE, Vector2)
	PRI(FontTileTextureSize, NONE, Vector2)
	PRI(FontMaterial, NONE, Material *)

	static Vector2 smDefaultFontSize;

	PRI(UIBuilder, GETREF, UIBuilder)

	PRI(FocusedElement, NONE, UIElement *);

public:
	void init();
	void terminate();

	// fonts

	const Vector2 &getCharTextureCoordinates(char character);
	const Vector2 &getFontTileTextureSize() const { return mFontTileTextureSize; }
	Material *getFontMaterial();
	static const Vector2 &getDefaultFontSize() { return smDefaultFontSize; }

	// groups

	const UIGroup& getGroup(const std::string & groupName) const;
	void addToGroup(const std::string & groupName, UIElement * uiElement);
	void removeFromGroup(const std::string & groupName, UIElement * uiElement);
	void removeElementsFromGroup(const std::string & groupName);
	void setGroupVisibility(const std::string & groupName, bool visibility);

	// focus

	UIElement *getFocusedElement() const { return mFocusedElement; };
	void setFocusedElement(UIElement * focusedElement);
};