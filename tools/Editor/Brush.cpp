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

	SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [&](const Event *event)
	{
		mMode = BrushMode::SELECT;
	});

    SUBSCRIBE_TO_EVENT(InputEventMouseButtonPressed, nullptr, this, [&](const Event *event)
	{
		InputEventMouseButtonPressed* e = (InputEventMouseButtonPressed*) event;

		if(e->mButton == GLFW_MOUSE_BUTTON_RIGHT)
		{
			mPaintMode = BrushPaintMode::ERASE;
		}
		else
		{
			mPaintMode = BrushPaintMode::PAINT;
		}
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonHold, nullptr, this, [&](const Event *event)
	{
		onPressed();
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [&](const Event *event)
	{
		onMouseMoved();
	});
}

void Brush::onPressed()
{
	if(mEditorController->canUseBrush())
	{
		Vector2 mouse = Input::getInstance()->getMousePosition();
		Vector3 worldPosition = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
		getFirstComponent<Camera>()->screenToWorld(mouse);

		if(mEditorController->getGrid().isInGrid(worldPosition))
		{
			Vector2 gridPosition = mEditorController->getGrid().calculateGridPosition(worldPosition);
			
			switch (mMode)
			{
				case BrushMode::SELECT:
				{

					break;
				}
				case BrushMode::PAINT:
				{
					switch (mPaintMode)
					{
						case BrushPaintMode::PAINT:
						{
							if(!mEditorController->getGrid().hasTile(gridPosition))
							{
								mEditorController->getGrid().setCell(gridPosition, 
									mEditorController->createTile(
										mEditorController->getGrid().calculateClampedPosition(worldPosition),
										mEditorController->getGrid().getTileSize(),
										mPaintData.mMaterial,
										mPaintData.mRegion
									)
								);
							}
							break;
						}
						case BrushPaintMode::ERASE:
						{
							if(mEditorController->getGrid().hasTile(gridPosition))
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
		}
	}
}

void Brush::onMouseMoved()
{
	if(mEditorController->canUseBrush())
	{
		setSelectorVisibility(true);

		Vector2 mouse = Input::getInstance()->getMousePosition();
		Vector3 worldPosition = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
		getFirstComponent<Camera>()->screenToWorld(mouse);

		mSelector->getTransform()->setLocalPosition(mEditorController->getGrid().calculateClampedPosition(worldPosition));

		if(mEditorController->getGrid().isInGrid(worldPosition))
		{
			mSelector->getFirstComponent<Renderer>()->setColor(Vector4(0,1,0,1));
		}
		else
		{
			mSelector->getFirstComponent<Renderer>()->setColor(Vector4(1,0,0,1));
		}
	}
	else
	{
		setSelectorVisibility(false);
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