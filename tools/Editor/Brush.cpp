#include "Brush.hpp"

#include "Input/Input.hpp"
#include "Input/InputEvents.hpp"
#include "Events/EventsManager.hpp"
#include "Maths/Vector3.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/Transform.hpp"

void Brush::init()
{
    SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event *event)
	{
		ECHO("BRUSH PRESSED");

		Vector2 mouse = Input::getInstance()->getMousePosition();
		Vector3 world = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
		getFirstComponent<Camera>()->screenToWorld(mouse);

		Vector3 clampedPosition(std::roundf(world.x / 50) * 50, std::roundf(world.y / 50) * 50, 0);

		onPressed(clampedPosition);
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this, [this](const Event *event)
	{

	});

	/*SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [this](const Event *event)
	{

	});*/
}

void Brush::onPressed(const Vector2& position)
{
	createSprite(position);
}

void Brush::createSprite(const Vector2 &position)
{
	Vector2 size(50, 50);

	Material *material = MaterialManager::getInstance()->loadMaterial("resources/terrain.png");

	GameObject *sprite = NEW(GameObject);
	sprite->init();

	sprite->getTransform()->setLocalPosition(position);
	sprite->getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer *renderer = NEW(Renderer);
	sprite->addComponent<Renderer>(renderer);

	//renderer->setColor(Vector4(0,0,0,0.7f));

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);
	renderer->addAnimation("idle", Animation::create(10, true, false, Vector2(0, 0), 1.0f / 16.0f, 1.0f / 16.0f, 10));
	renderer->setAnimation("idle");

	//renderer->setIsLineMode(true);

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(sprite);
}