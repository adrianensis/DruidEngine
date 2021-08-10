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

Vector2 Brush::getMouseWorldPosition() const
{
	Vector2 mouse = Input::getInstance()->getMousePosition();
	Vector2 worldPosition = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
	getFirstComponent<Camera>()->screenToWorld(mouse);

	return worldPosition;
}

void Brush::init(EditorController* editorController)
{
	mEditorController = editorController;
	createSelector();

	SUBSCRIBE_TO_EVENT(InputEventKeyEsc, nullptr, this, [&](const Event *event)
	{
		setModeSelect();
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonHold, nullptr, this, [&](const Event *event)
	{
		onHold();
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [&](const Event *event)
	{
		onMouseMoved();
	});
}

void Brush::onHold()
{
	if(Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if(mEditorController->canUseBrush())
		{
			Vector2 worldPosition = getMouseWorldPosition();

			if(mEditorController->getGrid().isInGrid(worldPosition))
			{
				Vector2 gridPosition = mEditorController->getGrid().calculateGridPosition(worldPosition);
							
				switch (mMode)
				{
					case BrushMode::SELECT:
					{
						const UIStyleEditor& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditor>();

						if( ! Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT_CONTROL))
						{
							mEditorController->forEachSelectedTile([&](GameObject* tile)
							{	
								if(tile->isActive())
								{
									Renderer* renderer = tile->getFirstComponent<Renderer>();
									renderer->setColor(style.mColor);
								}
							});

							mEditorController->getSelectedTiles().clear();
						}

						if(mEditorController->getGrid().hasTile(gridPosition))
						{
							GameObject* tile = mEditorController->getGrid().getCell(gridPosition).mGameObject;
							
							if(tile->isActive())
							{
								Renderer* renderer = tile->getFirstComponent<Renderer>();
								renderer->setColor(style.mColorPressed);

								mEditorController->getSelectedTiles().push_back(tile);
							}
						}

						break;
					}
					case BrushMode::PAINT:
					{
						switch (mPaintMode)
						{
							case BrushPaintMode::PAINT:
							{
								paintTile(worldPosition, gridPosition);
								break;
							}
							case BrushPaintMode::ERASE:
							{
								removeTile(gridPosition);
								break;
							}
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
		Vector2 worldPosition = getMouseWorldPosition();

		Vector2 clampedPosition = mEditorController->getGrid().calculateClampedPosition(worldPosition);

		if(mBrushPreview)
		{
			mBrushPreview->getTransform()->setLocalPosition(clampedPosition);
			setBrushPreviewVisibility(true);
			setSelectorVisibility(false);

			const UIStyleEditorBrushPreview& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorBrushPreview>();

			if(mEditorController->getGrid().isInGrid(worldPosition))
			{
				mBrushPreview->getFirstComponent<Renderer>()->setColor(style.mColor);
			}
			else
			{
				mBrushPreview->getFirstComponent<Renderer>()->setColor(style.mColor + Vector4(1,0,0,0));
			}
		}
		else
		{
			mSelector->getTransform()->setLocalPosition(clampedPosition);

			if(mEditorController->getGrid().isInGrid(worldPosition))
			{
				mSelector->getFirstComponent<Renderer>()->setColor(Vector4(0,1,0,1));
			}
			else
			{
				mSelector->getFirstComponent<Renderer>()->setColor(Vector4(1,0,0,1));
			}
			
			setSelectorVisibility(true);
		}
	}
	else
	{
		setSelectorVisibility(false);
		setBrushPreviewVisibility(false);
	}
}

void Brush::onTileSelectedFromAtlas(GameObject* tile)
{
	Renderer* tileRenderer = tile->getFirstComponent<Renderer>();
	mPaintData.mRegion = tileRenderer->getRegion();
	mPaintData.mMaterial = tileRenderer->getMaterial();

	setModePaint();
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

void Brush::setBrushPreviewVisibility(bool visible)
{
	if(mBrushPreview)
	{
		mBrushPreview->setIsActive(visible);
	}
}

void Brush::createBrushPreview()
{
	if(mBrushPreview)
	{
		removeBrushPreview();
	}

	if(mPaintData.mMaterial)
	{
		GameObject *brushPreview = NEW(GameObject);
		brushPreview->init();

		brushPreview->getTransform()->setLocalPosition(Vector2(0,0));
		brushPreview->getTransform()->setScale(Vector3(mEditorController->getGrid().getTileSize(), 1));

		Renderer *renderer = NEW(Renderer);
		brushPreview->addComponent<Renderer>(renderer);

		renderer->setMesh(Mesh::getRectangle());
		renderer->setLayer(0);

		renderer->setMaterial(mPaintData.mMaterial);
		renderer->setRegion(mPaintData.mRegion);

		const UIStyleEditorBrushPreview& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorBrushPreview>();

		renderer->setColor(style.mColor);

		ScenesManager::getInstance()->getCurrentScene()->addGameObject(brushPreview);

		mBrushPreview = brushPreview;

		setBrushPreviewVisibility(mEditorController->canUseBrush());
	}
}

void Brush::removeBrushPreview()
{
	if(mBrushPreview)
	{
		ScenesManager::getInstance()->getCurrentScene()->removeGameObject(mBrushPreview);
		mBrushPreview = nullptr;
	}
}

void Brush::paintTile(const Vector2 &worldPosition, const Vector2 &gridPosition)
{
	if(mEditorController->getGrid().hasTile(gridPosition))
	{
		removeTile(gridPosition);
	}

	mEditorController->getGrid().setCell(gridPosition, 
		mEditorController->createTile(
			mEditorController->getGrid().calculateClampedPosition(worldPosition),
			mEditorController->getGrid().getTileSize(),
			mPaintData.mMaterial,
			mPaintData.mRegion
		)
	);
}

void Brush::removeTile(const Vector2 &gridPosition)
{
	if(mEditorController->getGrid().hasTile(gridPosition))
	{
		CellGrid& cell = mEditorController->getGrid().getCell(gridPosition);
		ScenesManager::getInstance()->getCurrentScene()->removeGameObject(cell.mGameObject);
		cell.mGameObject = nullptr;
	}
}

void Brush::setModeSelect()
{
	mMode = BrushMode::SELECT;
	removeBrushPreview();
}

void Brush::setModePaint()
{
	if(mPaintData.mMaterial)
	{
		mMode = BrushMode::PAINT;
		mPaintMode = BrushPaintMode::PAINT;
		createBrushPreview();
	}
}

void Brush::setModeErase()
{
	mMode = BrushMode::PAINT;
	mPaintMode = BrushPaintMode::ERASE;
}