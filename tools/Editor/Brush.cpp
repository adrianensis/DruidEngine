#include "Brush.hpp"
#include "EditorController.hpp"

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

void Brush::init(EditorController* editorController)
{
	mEditorController = editorController;
	createSelector();

    SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [this](const Event *event)
	{
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

		mSelector->getTransform()->setLocalPosition(mEditorController->getGrid().calculateClampedPosition(world));

		if(mEditorController->getGrid().isInGrid(world))
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
	if(mEditorController->getGrid().isInGrid(position))
	{
		Vector2 gridPosition = mEditorController->getGrid().calculateGridPosition(position);
		
		switch (mMode)
		{
			case BrushMode::NONE:
			{

				break;
			}
			case BrushMode::SELECT:
			{

				break;
			}
			case BrushMode::PAINT:
			{
				if(!mEditorController->getGrid().hasTile(gridPosition))
				{
					mEditorController->getGrid().setCell(gridPosition, 
						mEditorController->createTile(
							mEditorController->getGrid().calculateClampedPosition(position),
							mEditorController->getGrid().getTileSize(),
							mPaintData.mMaterial,
							mPaintData.mRegion
						)
					);
				}
				break;
			}
			case BrushMode::ERASE:
			{
				if(!mEditorController->getGrid().hasTile(gridPosition))
				{
					CellGrid& cell = mEditorController->getGrid().getCell(gridPosition);
					ScenesManager::getInstance()->getCurrentScene()->removeGameObject(cell.mGameObject);
					cell.mGameObject = nullptr;
				}
				break;
			}
		}
	}
}

void Brush::onTileSelectedFromAtlas(GameObject* tile)
{
	mMode = BrushMode::PAINT;

	Renderer* tileRenderer = tile->getFirstComponent<Renderer>();
	mPaintData.mRegion = tileRenderer->getRegion();
	mPaintData.mMaterial = tileRenderer->getMaterial();
}

void Brush::createSelector()
{
	Material *material = MaterialManager::getInstance()->loadMaterial("resources/editor-icons/Selector.png");

	GameObject *selector = NEW(GameObject);
	selector->init();

	selector->getTransform()->setLocalPosition(Vector2(0,0));
	selector->getTransform()->setScale(Vector3(mEditorController->getGrid().getTileSize(), 1));

	Renderer *renderer = NEW(Renderer);
	selector->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(selector);

	mSelector = selector;
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

	GameObject *brushPreview = NEW(GameObject);
	brushPreview->init();

	brushPreview->getTransform()->setLocalPosition(Vector2(0,0));
	brushPreview->getTransform()->setScale(Vector3(mEditorController->getGrid().getTileSize(), 1));

	Renderer *renderer = NEW(Renderer);
	brushPreview->addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);

	ScenesManager::getInstance()->getCurrentScene()->addGameObject(brushPreview);

	mBrushPreview = brushPreview;
}

void Brush::removeBrushPreview()
{
	ScenesManager::getInstance()->getCurrentScene()->removeGameObject(mBrushPreview);
}