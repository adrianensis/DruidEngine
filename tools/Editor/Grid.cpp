#include "Grid.hpp"

#include "Maths/MathUtils.hpp"

void Grid::init()
{
    mGridSize.set(20, 20);
    mTileSize.set(50, 50);

    mCellsGrid.reserve(mGridSize.x);
    mCellsGrid.resize(mGridSize.x);

    FOR_ARRAY(i, mCellsGrid)
    {
        mCellsGrid[i].reserve(mGridSize.y);
        mCellsGrid[i].resize(mGridSize.y);
    }
}

Vector2 Grid::calculateGridPosition(const Vector2& position) const
{
    Vector3 clampedPosition(calculateClampedPosition(position));

    Vector2 gridPosition(
        (clampedPosition.x / mTileSize.x) + (mGridSize.x / 2.0f),
        (clampedPosition.y / mTileSize.y) + (mGridSize.y / 2.0f));

    return gridPosition;
}

Vector2 Grid::calculateClampedPosition(const Vector2& position) const
{
    Vector3 clampedPosition(std::roundf(position.x / mTileSize.x) * mTileSize.x, std::roundf(position.y / mTileSize.y) * mTileSize.y, 0);

    return clampedPosition;
}

void Grid::setCell(const Vector2& gridPosition, GameObject* tile)
{
    mCellsGrid[gridPosition.x][gridPosition.y].mGameObject = tile;
}

CellGrid& Grid::getCell(const Vector2& gridPosition)
{
    return mCellsGrid[gridPosition.x][gridPosition.y];
}

bool Grid::isInGrid(const Vector2& position) const 
{
    Vector2 gridPosition = calculateGridPosition(position);

    return gridPosition.x >= 0 && gridPosition.x < (mGridSize.x) && gridPosition.y >= 0 && gridPosition.y < (mGridSize.y);
}

bool Grid::hasTile(const Vector2& gridPosition) const
{
    return mCellsGrid[gridPosition.x][gridPosition.y].mGameObject;
}