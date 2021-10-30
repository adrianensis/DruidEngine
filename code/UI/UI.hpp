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
	PRI_M(SMap(SStr, UIGroup *), Groups, NONE)
	PRI_M(SMap(char, Vector2), CharMap, NONE)
	PRI_M(Vector2, FontTilesCount, NONE)
	PRI_M(Vector2, FontTileTextureSize, NONE)
	PRI_M(Material *, FontMaterial, NONE)

	static Vector2 smDefaultFontSize;

	PRI_M(UIBuilder, UIBuilder, GETREF)

	PRI_M(UIElement *, FocusedElement, NONE);

PUB
	void init();
	void terminate();

	// fonts

	const Vector2 &getCharTextureCoordinates(char character);
	const Vector2 &getFontTileTextureSize() const { return mFontTileTextureSize; }
	Material *getFontMaterial();
	static const Vector2 &getDefaultFontSize() { return smDefaultFontSize; }

	// groups

	const UIGroup& getGroup(const SStr & groupName) const;
	void addToGroup(const SStr & groupName, UIElement * uiElement);
	void removeFromGroup(const SStr & groupName, UIElement * uiElement);
    void destroyElementInGroup(const SStr & groupName, UIElement * uiElement);
	void destroyAllElementsInGroup(const SStr & groupName);
	void setGroupVisibility(const SStr & groupName, bool visibility);

	// focus

	UIElement *getFocusedElement() const { return mFocusedElement; };
	void setFocusedElement(UIElement * focusedElement);
};