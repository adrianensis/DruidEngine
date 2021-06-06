#include "Playground.hpp"

#include "Core/Time/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
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

	
}

void Playground::step() {

	if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)) {
		createSprite(Input::getInstance()->getMousePosition() * 800, 0);
	}

	if (Input::getInstance()->isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)) {
		createSprite(Input::getInstance()->getMousePosition() * 800, 1);
	}
}

void Playground::terminate() {

}