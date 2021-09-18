#include "Grid.hpp"
#include "../EditorController.hpp"

#include "Maths/MathUtils.hpp"

void Grid::init(EditorController* editorController)
{
    mEditorController = editorController;
    
    mGridSize.set(50, 50);
    mTileSize.set(50, 50);

    mCellsGrid.resize(mGridSize.x);

    FOR_ARRAY(i, mCellsGrid)
    {
        mCellsGrid[i].resize(mGridSize.y);
    }
}

Vector2 Grid::calculateGridPosition(CNS Vector2& worldPosition) CNS
{
    Vector3 clampedPosition(calculateClampedPosition(worldPosition));

    Vector2 gridPosition(
        (clampedPosition.x / mTileSize.x) + (mGridSize.x / 2.0f),
        (clampedPosition.y / mTileSize.y) + (mGridSize.y / 2.0f));

    return gridPosition;
}

Vector2 Grid::calculateClampedPosition(CNS Vector2& worldPosition) CNS
{
    Vector3 clampedPosition(std::roundf(worldPosition.x / mTileSize.x) * mTileSize.x, std::roundf(worldPosition.y / mTileSize.y) * mTileSize.y, 0);

    return clampedPosition;
}

void Grid::setCell(CNS Vector2& gridPosition, GameObject* tile)
{
    mCellsGrid[gridPosition.x][gridPosition.y].mGameObject = tile;
}

CellGrid& Grid::getCell(CNS Vector2& gridPosition)
{
    return mCellsGrid[gridPosition.x][gridPosition.y];
}

bool Grid::isInGrid(CNS Vector2& worldPosition) CNS 
{
    Vector2 gridPosition = calculateGridPosition(worldPosition);

    return gridPosition.x >= 0 && gridPosition.x < (mGridSize.x) && gridPosition.y >= 0 && gridPosition.y < (mGridSize.y);
}

bool Grid::hasTile(CNS Vector2& gridPosition) CNS
{
    return mCellsGrid[gridPosition.x][gridPosition.y].mGameObject;
}