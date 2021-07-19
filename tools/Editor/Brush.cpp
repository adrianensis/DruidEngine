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

#include "Grid.hpp"

void Brush::init()
{
    SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event *event)
	{
		ECHO("BRUSH PRESSED");

		Vector2 mouse = Input::getInstance()->getMousePosition();
		Vector3 world = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
		getFirstComponent<Camera>()->screenToWorld(mouse);

		onPressed(world);
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonReleased, nullptr, this, [this](const Event *event)
	{

	});

	SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [this](const Event *event)
	{
		Vector2 mouse = Input::getInstance()->getMousePosition();
		Vector3 world = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
		getFirstComponent<Camera>()->screenToWorld(mouse);

		mSelector->getTransform()->setLocalPosition(mGrid->calculateClampedPosition(world));

		if(mGrid->isInGrid(world))
		{
			mSelector->getFirstComponent<Renderer>()->setColor(Vector4(0,1,0,1));
		}
		else
		{
			mSelector->getFirstComponent<Renderer>()->setColor(Vector4(1,0,0,1));
		}
	});
}

void Brush::onPressed(const Vector2& position)
{
	if(mGrid->isInGrid(position))
	{
		Vector2 gridPosition = mGrid->calculateGridPosition(position);
		if(!mGrid->hasTile(gridPosition))
		{
			mGrid->setCell(gridPosition, createSprite(mGrid->calculateClampedPosition(position), mGrid->getTileSize()));
		}
	}
}

GameObject* Brush::createSprite(const Vector2 &position, const Vector2 &size)
{
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

	return sprite;
}

void Brush::createSelector()
{
	Material *material = MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Selector.png");

	GameObject *sprite = NEW(GameObject);
	sprite->init();

	sprite->getTransform()->setLocalPosition(Vector2(0,0));
	sprite->getTransform()->setScale(Vector3(mGrid->getTileSize(), 1));

	Renderer *renderer = NEW(Renderer);
	sprite->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(sprite);

	mSelector = sprite;
}

void Brush::setSelectorVisibility(bool visible)
{
	mSelector->setIsActive(visible);
}

void Brush::createBrushPreview()
{
	if(mBrushPreview)
	{
		removeBrushPreview();
	}

	Material *material = MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Selector.png");

	GameObject *sprite = NEW(GameObject);
	sprite->init();

	sprite->getTransform()->setLocalPosition(Vector2(0,0));
	sprite->getTransform()->setScale(Vector3(mGrid->getTileSize(), 1));

	Renderer *renderer = NEW(Renderer);
	sprite->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(sprite);

	mBrushPreview = sprite;
}

void Brush::removeBrushPreview()
{
	ScenesManager::getInstance()->getCurrentScene()->removeGameObject(mBrushPreview);
}

void Brush::setGrid(Grid* grid)
{
	mGrid = grid;

	createSelector();
}