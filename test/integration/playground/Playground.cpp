#include "Playground.hpp"

#include "Core/Time/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "UI/UI.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"

#include "ProjectileScript.hpp"

void Playground::createSprite(const Vector2& position, u32 type/* = 0*/) {
	Vector2 size(150, 150);

	Material* material = MaterialManager::getInstance()->loadMaterial("resources/terrain.png");

	GameObject* sprite = NEW(GameObject);
	sprite->init();

	sprite->getTransform()->setLocalPosition(position);
	sprite->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = NEW(Renderer);
	sprite->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	switch (type)
	{
	case 0:
		renderer->setMaterial(material);
		renderer->addAnimation("idle", Animation::create(10, true, false, Vector2(0, 0), 1.0f/16.0f, 1.0f/16.0f, 10));
		renderer->setAnimation("idle");
		break;
	case 1:
		renderer->setMaterial(material);
		renderer->addAnimation("idle", Animation::create(8, true, false, Vector2(8.0f/16.0f, 10.0f/16.0f), 1.0f/16.0f, 1.0f/16.0f, 3));
		renderer->setAnimation("idle");
		break;
	}

	

	//renderer->setIsLineMode(true);

	getGameObject()->getScene()->addGameObject(sprite);
}

void Playground::init() {
	mTransform = getGameObject()->getTransform();
	mMaterial = nullptr;
}

void Playground::firstStep() {

	/*UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL)->
	setPosition(Vector2(0,0))->
	setLayer(0)->
	setAdjustSizeToText(false)->
	setSize(Vector2(0.1f, 0.1f))->
	create(UIElementType::PANEL)->
	getUIElement();*/

	mUIText = (UIText*) UI::getInstance()->getUIBuilder().
	setLayout(UILayout::VERTICAL)->
	setPosition(Vector2(0.5,0.5f))->
	setLayer(0)->
	setAdjustSizeToText(false)->
	setSize(Vector2(0.1f, 0.1f))->
	setText("hello")->
	create(UIElementType::TEXT)->
	getUIElement();
}

void Playground::step() {

	if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		createSprite(Input::getInstance()->getMousePosition() * 800, 0);
		mUIText->setText("left");
	}

	if (Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		createSprite(Input::getInstance()->getMousePosition() * 800, 1);
		mUIText->setText("right  123");
	}
}

void Playground::terminate() {

}