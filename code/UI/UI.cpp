#include "UI/UI.hpp"
#include "UI/UIGroup.hpp"
#include "UI/UIStyle.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Material/Material.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Scene/Transform.hpp"
#include "Input/Input.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"
#include "Events/EventsManager.hpp"
#include "Scene/GameObject.hpp"

Vector2 UI::smDefaultFontSize = Vector2(0.04f / 2.0f, 0.04f);

Material *UI::getFontMaterial()
{
	if (!mFontMaterial)
	{
		mFontMaterial = MaterialManager::getInstance()->loadMaterial("resources/font16x16.png");
	}
	return mFontMaterial;
};

UIGroup& UI::getOrCreateGroup(const std::string & groupName)
{
	if (!MAP_CONTAINS(mGroups, groupName))
	{
		UIGroup *group = NEW(UIGroup);
		group->init();
		group->mName = groupName;

		MAP_INSERT(mGroups, groupName, group);
	}

	return *(mGroups.at(groupName));
}

void UI::init()
{
	UIStyleManager::getInstance()->init();

	mFontTilesCount = Vector2(16.0f, 6.0f);
	mFontTileTextureSize = Vector2(1.0f / mFontTilesCount.x, 1.0f / mFontTilesCount.y);

	MAP_INSERT(mCharMap, ' ', Vector2(0 / mFontTilesCount.x, 5 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, '!', Vector2(1 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '\"', Vector2(2 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '#', Vector2(3 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '$', Vector2(4 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '%', Vector2(5 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '&', Vector2(6 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '(', Vector2(8 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, ')', Vector2(9 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '*', Vector2(10 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '+', Vector2(11 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, ',', Vector2(12 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '-', Vector2(13 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '.', Vector2(14 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '/', Vector2(15 / mFontTilesCount.x, 5 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, '0', Vector2(0 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '1', Vector2(1 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '2', Vector2(2 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '3', Vector2(3 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '4', Vector2(4 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '5', Vector2(5 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '6', Vector2(6 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '7', Vector2(7 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '8', Vector2(8 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '9', Vector2(9 / mFontTilesCount.x, 4 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, ':', Vector2(10 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, ';', Vector2(11 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '<', Vector2(12 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '=', Vector2(13 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '>', Vector2(14 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '?', Vector2(15 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '@', Vector2(0 / mFontTilesCount.x, 3 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, 'A', Vector2(1 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'B', Vector2(2 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'C', Vector2(3 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'D', Vector2(4 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'E', Vector2(5 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'F', Vector2(6 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'G', Vector2(7 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'H', Vector2(8 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'I', Vector2(9 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'J', Vector2(10 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'K', Vector2(11 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'L', Vector2(12 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'M', Vector2(13 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'N', Vector2(14 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'O', Vector2(15 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'P', Vector2(0 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'Q', Vector2(1 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'R', Vector2(2 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'S', Vector2(3 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'T', Vector2(4 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'U', Vector2(5 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'V', Vector2(6 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'W', Vector2(7 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'X', Vector2(8 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'Y', Vector2(9 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'Z', Vector2(10 / mFontTilesCount.x, 2 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, '[', Vector2(11 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '\\', Vector2(12 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, ']', Vector2(13 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '^', Vector2(14 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '_', Vector2(15 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '`', Vector2(0 / mFontTilesCount.x, 1 / mFontTilesCount.y));

	MAP_INSERT(mCharMap, 'a', Vector2(1 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'b', Vector2(2 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'c', Vector2(3 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'd', Vector2(4 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'e', Vector2(5 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'f', Vector2(6 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'g', Vector2(7 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'h', Vector2(8 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'i', Vector2(9 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'j', Vector2(10 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'k', Vector2(11 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'l', Vector2(12 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'm', Vector2(13 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'n', Vector2(14 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'o', Vector2(15 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'p', Vector2(0 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'q', Vector2(1 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'r', Vector2(2 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 's', Vector2(3 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 't', Vector2(4 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'u', Vector2(5 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'v', Vector2(6 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'w', Vector2(7 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'x', Vector2(8 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'y', Vector2(9 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, 'z', Vector2(10 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '{', Vector2(11 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '|', Vector2(12 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '}', Vector2(13 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	MAP_INSERT(mCharMap, '~', Vector2(14 / mFontTilesCount.x, 0 / mFontTilesCount.y));
}

const Vector2 &UI::getCharTextureCoordinates(char character)
{
	return mCharMap.at(character);
}

void UI::setFocusedElement(UIElement *focusedElement)
{
	mFocusedElement = focusedElement;
};

void UI::terminate()
{
	TRACE()

	MAP_DELETE_CONTENT(mGroups)

	UIStyleManager::deleteInstance();
}