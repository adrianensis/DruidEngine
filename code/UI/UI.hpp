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
	PRI(Groups, NONE, SMap<SStr, UIGroup *>)
	PRI(CharMap, NONE, SMap<char, Vector2>)
	PRI(FontTilesCount, NONE, Vector2)
	PRI(FontTileTextureSize, NONE, Vector2)
	PRI(FontMaterial, NONE, Material *)

	STC Vector2 smDefaultFontSize;

	PRI(UIBuilder, GETREF, UIBuilder)

	PRI(FocusedElement, NONE, UIElement *);

public:
	void init();
	void terminate();

	// fonts

	CNS Vector2 &getCharTextureCoordinates(char character);
	CNS Vector2 &getFontTileTextureSize() CNS { return mFontTileTextureSize; }
	Material *getFontMaterial();
	STC CNS Vector2 &getDefaultFontSize() { return smDefaultFontSize; }

	// groups

	CNS UIGroup& getGroup(CNS SStr & groupName) CNS;
	void addToGroup(CNS SStr & groupName, UIElement * uiElement);
	void removeFromGroup(CNS SStr & groupName, UIElement * uiElement);
	void removeElementsFromGroup(CNS SStr & groupName);
	void setGroupVisibility(CNS SStr & groupName, bool visibility);

	// focus

	UIElement *getFocusedElement() CNS { return mFocusedElement; };
	void setFocusedElement(UIElement * focusedElement);
};