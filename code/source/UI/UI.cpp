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
#include "List.hpp"
#include "HashMap.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UI::UI() :
		DE_Class(), Singleton() {
	mButtonTexture = nullptr;
	mButtonMaterial = nullptr;
	mFontTexture = nullptr;
	mFontMaterial = nullptr;
	mUIElements = nullptr;
}

UI::~UI() = default;

// ---------------------------------------------------------------------------

UIButton* UI::createButton(Scene *scene, const Vector2 &position,
		const Vector2 &size, u32 layer) {

	if (!mButtonMaterial) {
		mButtonMaterial = MaterialManager::getInstance()->loadMaterial(
				"resources/button.bmp");
	}

	UIButton *uiButton = Memory::allocate<UIButton>();
	uiButton->init();

	uiButton->getTransform()->setLocalPosition(position);
	uiButton->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer *renderer = Memory::allocate<Renderer>();
	uiButton->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mButtonMaterial);
	renderer->setLayer(layer);

	renderer->setAffectedByProjection(false);

	RigidBody *rigidBody = Memory::allocate<RigidBody>();
	uiButton->addComponent<RigidBody>(rigidBody);
	rigidBody->setSimulate(false);

	Collider *collider = Memory::allocate<Collider>();
	uiButton->addComponent<Collider>(collider);
	collider->setSize(size.x, size.y);
	collider->getBoundingBox();

	uiButton->setComponentsCache();

	uiButton->setIsStatic(true);

	scene->addGameObject(uiButton);

	mUIElements->pushBack(uiButton);

	return uiButton;
}

// ---------------------------------------------------------------------------

UIText* UI::createText(Scene *scene, const Vector2 &position,
		const Vector2 &size, const std::string &text, u32 layer) {

	if (!mFontMaterial) {
		mFontMaterial = MaterialManager::getInstance()->loadMaterial(
				"resources/font16x16.png");
	}

	UIText *uiText = Memory::allocate<UIText>();
	uiText->init();

	uiText->getTransform()->setLocalPosition(position);
	uiText->getTransform()->setScale(Vector3(size.x, size.y, 1));

	uiText->setSize(size);
	uiText->setLayer(layer);
	uiText->setText(text);

	uiText->setComponentsCache();

	uiText->setIsStatic(true);

	scene->addGameObject(uiText);

	mUIElements->pushBack(uiText);

	return uiText;
}

// ---------------------------------------------------------------------------

UIList* UI::createList(Scene *scene, const Vector2 &position,
		const Vector2 &size, u32 layer) {

	UIList *uiList = Memory::allocate<UIList>();
	uiList->init();

	uiList->getTransform()->setLocalPosition(position);
	uiList->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer *renderer = Memory::allocate<Renderer>();
	uiList->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(mButtonMaterial);
	renderer->setLayer(layer);

	uiList->setComponentsCache();

	uiList->setIsStatic(false);

	f32 halfWidth = size.x / 2.0f;
	f32 halfHeight = size.y / 2.0f;

	f32 margin = 50;
	f32 itemOffset = 50;

	FOR_RANGE(i, 0, 3)
	{
		createText(scene,
				position
						+ Vector3(-halfWidth + margin,
								halfHeight - margin + -itemOffset * i, 0),
				Vector2(40, 40), "list item", layer);
	}

	scene->addGameObject(uiList);

	// scrollbar

	f32 scrollbarMargin = 20;
	UIButton *upButton = createButton(scene,
			position
					+ Vector3(halfWidth + scrollbarMargin, -halfHeight / 2.0f,
							0), Vector2(40, halfHeight - scrollbarMargin),
			layer);

	upButton->setOnPressedCallback([&]() {

	});

	UIButton *downButton = createButton(scene,
			position
					+ Vector3(halfWidth + scrollbarMargin, halfHeight / 2.0f,
							0), Vector2(40, halfHeight - scrollbarMargin),
			layer);

	downButton->setOnPressedCallback([&]() {

	});

	mUIElements->pushBack(uiList);

	return uiList;
}

// ---------------------------------------------------------------------------

void UI::init() {
	mUIElements = Memory::allocate<List<UIElement*>>();
	mUIElements->init();

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

	if (Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)) {

		Vector2 screenMousePosition(Input::getMousePosition());
		Vector2 worldMousePosition = Vector2(
				RenderEngine::getInstance()->getCamera()->screenToWorld(
						screenMousePosition));

		bool pressed = false;

		FOR_LIST_COND(it, mUIElements, !pressed)
		{

			UIElement *element = it.get();

			Collider *collider = element->getCollider();
			Renderer *renderer = element->getRenderer();

			if (collider)
				collider->getBoundingBox(); // force regenerate bounding box

			Vector2 mousePosition =
					renderer->isAffectedByProjection() ?
							worldMousePosition : screenMousePosition;

			if (collider && !renderer->isOutOfCamera()
					&& collider->testPoint(mousePosition)
							== ColliderStatus::STATUS_PENETRATION) {
				element->onPressed();
				pressed = true;

				if (element->isConsumeInput()) {
					Input::clearMouseButton();
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------

void UI::terminate() {
	TRACE();
//  FOR_LIST(it, mUIElements){
//    Memory::free<UIElement>(it.get());
//  }

	Memory::free<HashMap<c8, Vector2>>(mCharMap);

	Memory::free<List<UIElement*>>(mUIElements);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
