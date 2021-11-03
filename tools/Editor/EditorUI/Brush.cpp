#include "Brush.hpp"
#include "../EditorController.hpp"

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
	Vector3 worldPosition = ScenesManager::getInstance()->getCurrentScene()->getCameraGameObject()->
	getFirstComponent<Camera>()->screenToWorld(mouse);

	return worldPosition;
}

void Brush::init(EditorController* editorController)
{
	EditorUIElement::init(editorController);

	createSelector();

	SUBSCRIBE_TO_EVENT(InputEventMouseButtonHold, nullptr, this, [&](const Event *event)
	{
		onHold();
	});

	SUBSCRIBE_TO_EVENT(InputEventMouseMoved, nullptr, this, [&](const Event *event)
	{
		onMouseMoved();
	});

	mMaxBrushSize = 10;
	mBrushSize = 1;
}

void Brush::onHold()
{
	if(Input::getInstance()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if(getEditorController()->canUseBrush())
		{
			Vector2 worldPosition = getMouseWorldPosition();

			FOR_RANGE(i, 0, mBrushSize)
			{
				FOR_RANGE(j, 0, mBrushSize)
				{
					Vector2 singleTileWorldPosition = worldPosition + 
					getEditorController()->getGrid().getTileSize() * Vector2(i,j);

					if(getEditorController()->getGrid().isInGrid(singleTileWorldPosition))
					{
						applyOneTile(singleTileWorldPosition);
					}
				}
			}
		}
	}
}

void Brush::applyOneTile(const Vector2 &worldPosition)
{
	Vector2 gridPosition = getEditorController()->getGrid().calculateGridPosition(worldPosition);
				
	switch (mMode)
	{
		case BrushMode::SELECT:
		{
			const UIStyleEditor& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditor>();

			if( ! Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			{
				getEditorController()->forEachSelectedTile([&](GameObject* tile)
				{	
					if(tile->isActive())
					{
						Renderer* renderer = tile->getFirstComponent<Renderer>();
						renderer->setColor(style.mBackgroundColor);
					}
				});

				getEditorController()->getSelectedTiles().clear();
			}

			if(getEditorController()->getGrid().hasTile(gridPosition))
			{
				GameObject* tile = getEditorController()->getGrid().getCell(gridPosition).mGameObject;
				
				if(tile->isActive())
				{
					Renderer* renderer = tile->getFirstComponent<Renderer>();
					renderer->setColor(style.mColorPressed);

					getEditorController()->getSelectedTiles().push_back(tile);
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

void Brush::onMouseMoved()
{
	if(getEditorController()->canUseBrush())
	{
		Vector2 worldPosition = getMouseWorldPosition();
		Vector2 clampedPosition = getEditorController()->getGrid().calculateClampedPosition(worldPosition);

		if(!mBrushPreview.empty())
		{
			FOR_RANGE(i, 0, mBrushSize)
			{
				FOR_RANGE(j, 0, mBrushSize)
				{
					Vector2 singleTileClampedPosition = clampedPosition + 
					getEditorController()->getGrid().getTileSize() * Vector2(i,j);

					u32 index = j + i*mBrushSize;
					mBrushPreview[index]->getTransform()->setLocalPosition(singleTileClampedPosition);
					const UIStyleEditorBrushPreview& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorBrushPreview>();

					if(getEditorController()->getGrid().isInGrid(singleTileClampedPosition))
					{
						mBrushPreview[index]->getFirstComponent<Renderer>()->setColor(style.mBackgroundColor);
					}
					else
					{
						mBrushPreview[index]->getFirstComponent<Renderer>()->setColor(style.mBackgroundColor + Vector4(1,0,0,0));
					}
				}
			}

			setBrushPreviewVisibility(true);
			setSelectorVisibility(false);
		}
		else
		{
			mSelector->getTransform()->setLocalPosition(clampedPosition);

			if(getEditorController()->getGrid().isInGrid(worldPosition))
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
	selector->getTransform()->setScale(Vector3(getEditorController()->getGrid().getTileSize(), 1));

	Renderer *renderer = NEW(Renderer);
	renderer->init();

	renderer->setMesh(Mesh::getRectangle());
	renderer->setLayer(0);

	renderer->setMaterial(material);

	selector->addComponent<Renderer>(renderer);
	
	ScenesManager::getInstance()->getCurrentScene()->addGameObject(selector);

	mSelector = selector;
}

void Brush::setSelectorVisibility(bool visible)
{
	mSelector->setIsActive(visible);
}

void Brush::setBrushPreviewVisibility(bool visible)
{
	FOR_ARRAY(i, mBrushPreview)
	{
		mBrushPreview[i]->setIsActive(visible);
	}
}

void Brush::createBrushPreview()
{
	removeBrushPreview();

	FOR_RANGE(i, 0, mBrushSize)
	{
		FOR_RANGE(j, 0, mBrushSize)
		{
			createBrushPreviewOneTile(Vector2(i, j));
		}
	}

	onMouseMoved();
}

void Brush::createBrushPreviewOneTile(const Vector2 &brushPreviewIndex)
{
	if(mPaintData.mMaterial)
	{
		GameObject *brushPreview = NEW(GameObject);
		brushPreview->init();

		brushPreview->getTransform()->setLocalPosition(brushPreviewIndex * getEditorController()->getGrid().getTileSize());
		brushPreview->getTransform()->setScale(Vector3(getEditorController()->getGrid().getTileSize(), 1));

		Renderer *renderer = NEW(Renderer);
		renderer->init();

		renderer->setMesh(Mesh::getRectangle());
		renderer->setLayer(0);

		renderer->setMaterial(mPaintData.mMaterial);
		renderer->setRegion(mPaintData.mRegion);

		const UIStyleEditorBrushPreview& style = UIStyleManager::getInstance()->getOrAddStyle<UIStyleEditorBrushPreview>();

		renderer->setColor(style.mBackgroundColor);

		brushPreview->addComponent<Renderer>(renderer);

		ScenesManager::getInstance()->getCurrentScene()->addGameObject(brushPreview);

		mBrushPreview.push_back(brushPreview);

		setBrushPreviewVisibility(getEditorController()->canUseBrush());
	}
}

void Brush::removeBrushPreview()
{
	FOR_ARRAY(i, mBrushPreview)
	{
		ScenesManager::getInstance()->getCurrentScene()->removeGameObject(mBrushPreview[i]);
	}

	mBrushPreview.clear();
}

void Brush::paintTile(const Vector2 &worldPosition, const Vector2 &gridPosition)
{
	if(getEditorController()->getGrid().hasTile(gridPosition))
	{
		removeTile(gridPosition);
	}

	getEditorController()->getGrid().setCell(gridPosition, 
		getEditorController()->createTile(
			getEditorController()->getGrid().calculateClampedPosition(worldPosition),
			getEditorController()->getGrid().getTileSize(),
			mPaintData.mMaterial,
			mPaintData.mRegion
		)
	);
}

void Brush::removeTile(const Vector2 &gridPosition)
{
	if(getEditorController()->getGrid().hasTile(gridPosition))
	{
		CellGrid& cell = getEditorController()->getGrid().getCell(gridPosition);
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
	if(mPaintData.mMaterial)
	{
		mMode = BrushMode::PAINT;
		mPaintMode = BrushPaintMode::ERASE;
		createBrushPreview();
	}
}

void Brush::setBrushSize(u32 size)
{
	if(size > 0 && size <= mMaxBrushSize)
	{
		mBrushSize = size;

		createBrushPreview();
	}
}