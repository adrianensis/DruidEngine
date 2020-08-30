#include "UI.hpp"
#include "Memory.hpp"
#include "Texture.hpp"
#include "MaterialManager.hpp"
#include "Material.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "RenderEngine.hpp"
#include "RenderContext.hpp"
#include "List.hpp"
#include "HashMap.hpp"

namespace DE {

UIGroup::UIGroup() : DE_Class(){
	mName = "";
	mUIElements = nullptr;
	mVisible = true;
}

UIGroup::~UIGroup(){
	Memory::free<List<UIElement*>>(mUIElements);
}

void UIGroup::init(){
	mUIElements = Memory::allocate<List<UIElement*>>();
	mUIElements->init();
}

// ---------------------------------------------------------------------------

UI::UI() : DE_Class(), Singleton() {
	//mUIElements = nullptr;
	mFontMaterial = nullptr;
	mUIBuilder = nullptr;
}

UI::~UI() = default;

// ---------------------------------------------------------------------------

UIBuilder* UI::getBuilder() {
	if(!mUIBuilder){
		mUIBuilder = UIBuilder::getInstance();
	}
	return mUIBuilder;
}

// ---------------------------------------------------------------------------

Material* UI::getFontMaterial() {
	if (!mFontMaterial) {
		mFontMaterial = MaterialManager::getInstance()->loadMaterial("resources/font16x16.png");
	}
	return mFontMaterial;
};

// ---------------------------------------------------------------------------

void UI::addToGroup(const std::string& groupName, UIElement* uiElement) {

	if(!mGroups->contains(groupName)){
		UIGroup* group = Memory::allocate<UIGroup>();
		group->init();
		group->mName = groupName;

		mGroups->set(groupName, group);
	}

	mGroups->get(groupName)->mUIElements->pushBack(uiElement);
}

void UI::removeFromGroup(const std::string& groupName, UIElement* uiElement) {
	if(mGroups->contains(groupName)){
		mGroups->get(groupName)->mUIElements->remove(uiElement);
	}
}

void UI::removeElementsFromGroup(const std::string& groupName) {
	if(mGroups->contains(groupName)){
		UIGroup* group = mGroups->get(groupName);
		auto list = group->mUIElements;

		FOR_LIST(it, list){
			UIElement* element = it.get();
			element->getScene()->removeGameObject(element);
		}

		mGroups->get(groupName)->mUIElements->clear();
	}
}

void UI::setGroupVisibility(const std::string& groupName, bool visibility) {
	if(mGroups->contains(groupName)){
		UIGroup* group = mGroups->get(groupName);
		group->mVisible = visibility;

		FOR_LIST(it, mGroups->get(groupName)->mUIElements){
			it.get()->setVisibility(group->mVisible);
		}
	}
}

// ---------------------------------------------------------------------------

void UI::init() {
	/*mUIElements = Memory::allocate<List<UIElement*>>();
	mUIElements->init();*/

	mGroups = Memory::allocate<HashMap<std::string, UIGroup*>>();
	mGroups->init();

	mFontTilesCount = Vector2(16.0f, 6.0f);
	mFontTileTextureSize = Vector2(1.0f / mFontTilesCount.x, 1.0f / mFontTilesCount.y);

	mCharMap = Memory::allocate<HashMap<c8, Vector2>>();
	mCharMap->init();

	mCharMap->set(' ', Vector2(0 / mFontTilesCount.x, 0 / mFontTilesCount.y));

	mCharMap->set('!', Vector2(1 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('\"', Vector2(2 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('#', Vector2(3 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('$', Vector2(4 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('%', Vector2(5 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('&', Vector2(6 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('(', Vector2(8 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set(')', Vector2(9 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('*', Vector2(10 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('+', Vector2(11 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set(',', Vector2(12 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('-', Vector2(13 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('.', Vector2(14 / mFontTilesCount.x, 0 / mFontTilesCount.y));
	mCharMap->set('/', Vector2(15 / mFontTilesCount.x, 0 / mFontTilesCount.y));

	mCharMap->set('0', Vector2(0 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('1', Vector2(1 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('2', Vector2(2 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('3', Vector2(3 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('4', Vector2(4 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('5', Vector2(5 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('6', Vector2(6 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('7', Vector2(7 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('8', Vector2(8 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('9', Vector2(9 / mFontTilesCount.x, 1 / mFontTilesCount.y));

	mCharMap->set(':', Vector2(10 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set(';', Vector2(11 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('<', Vector2(12 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('=', Vector2(13 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('>', Vector2(14 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('?', Vector2(15 / mFontTilesCount.x, 1 / mFontTilesCount.y));
	mCharMap->set('@', Vector2(0 / mFontTilesCount.x, 2 / mFontTilesCount.y));

	mCharMap->set('A', Vector2(1 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('B', Vector2(2 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('C', Vector2(3 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('D', Vector2(4 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('E', Vector2(5 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('F', Vector2(6 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('G', Vector2(7 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('H', Vector2(8 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('I', Vector2(9 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('J', Vector2(10 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('K', Vector2(11 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('L', Vector2(12 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('M', Vector2(13 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('N', Vector2(14 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('O', Vector2(15 / mFontTilesCount.x, 2 / mFontTilesCount.y));
	mCharMap->set('P', Vector2(0 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('Q', Vector2(1 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('R', Vector2(2 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('S', Vector2(3 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('T', Vector2(4 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('U', Vector2(5 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('V', Vector2(6 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('W', Vector2(7 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('X', Vector2(8 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('Y', Vector2(9 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('Z', Vector2(10 / mFontTilesCount.x, 3 / mFontTilesCount.y));

	mCharMap->set('[', Vector2(11 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('\\', Vector2(12 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set(']', Vector2(13 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('^', Vector2(14 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('_', Vector2(15 / mFontTilesCount.x, 3 / mFontTilesCount.y));
	mCharMap->set('`', Vector2(0 / mFontTilesCount.x, 4 / mFontTilesCount.y));

	mCharMap->set('a', Vector2(1 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('b', Vector2(2 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('c', Vector2(3 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('d', Vector2(4 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('e', Vector2(5 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('f', Vector2(6 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('g', Vector2(7 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('h', Vector2(8 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('i', Vector2(9 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('j', Vector2(10 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('k', Vector2(11 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('l', Vector2(12 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('m', Vector2(13 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('n', Vector2(14 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('o', Vector2(15 / mFontTilesCount.x, 4 / mFontTilesCount.y));
	mCharMap->set('p', Vector2(0 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('q', Vector2(1 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('r', Vector2(2 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('s', Vector2(3 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('t', Vector2(4 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('u', Vector2(5 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('v', Vector2(6 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('w', Vector2(7 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('x', Vector2(8 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('y', Vector2(9 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('z', Vector2(10 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('{', Vector2(11 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('|', Vector2(12 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('}', Vector2(13 / mFontTilesCount.x, 5 / mFontTilesCount.y));
	mCharMap->set('~', Vector2(14 / mFontTilesCount.x, 5 / mFontTilesCount.y));
}

// ---------------------------------------------------------------------------

Vector2 UI::getCharTextureCoordinates(c8 character) {
	return mCharMap->get(character);
}

// ---------------------------------------------------------------------------

void UI::step() {

	// TODO : remove UI step
	if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)) {

	}
}

// ---------------------------------------------------------------------------

void UI::internalRemoveUIElement(const Iterator *it) {
	/*auto castedIt = it->cast<UIElement*>();
	mUIElements->remove(*castedIt);*/
}

// ---------------------------------------------------------------------------

void UI::terminate() {
	TRACE();

	if(mUIBuilder){
		Memory::free<UIBuilder>(mUIBuilder);
	}

	if(mCharMap){
		Memory::free<HashMap<c8, Vector2>>(mCharMap);
	}

	/*if(mUIElements){
		Memory::free<List<UIElement*>>(mUIElements);
	}*/

	if(mGroups){
		FOR_LIST(it, mGroups->getValues()){
			Memory::free<UIGroup>(it.get());
		}

		Memory::free<HashMap<std::string, UIGroup*>>(mGroups);
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
