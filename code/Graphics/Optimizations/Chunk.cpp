#include "Graphics/Optimizations/Chunk.hpp"

#include "Graphics/Material/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Optimizations/BatchesMap.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Log/Log.hpp"

Chunk::~Chunk()
{
	FOR_LIST(it, *mRenderers)
	{
		(*it)->finallyDestroy();
		DELETE((*it));
	}

	DELETE(mRenderers);
}

void Chunk::init()
{
	//TRACE();

	mRenderers = NEW(SLst<Renderer *>);

	mLeftTop.set(0, 0, 0);
}

void Chunk::set(CNS Vector3 &leftTop, f32 size)
{
	mLeftTop = leftTop;
	mSize = size;
	f32 halfSize = mSize / 2.0f;
	mCenter.set(mLeftTop.x + halfSize, mLeftTop.y - halfSize, 0);

	mRadius = mCenter.dst(mLeftTop);
}

void Chunk::update(BatchesMap *batchesMap)
{
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mSize,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

	FOR_LIST(it, *mRenderers)
	{
		Renderer *renderer = *it;

		bool removeFromList = false;

		if (renderer->isActive())
		{
			if (!renderer->getIsAlreadyInBatch())
			{
				batchesMap->addRenderer(renderer);
			}

			if (!renderer->isStatic() && !containsRenderer(renderer))
			{
				Chunk *newChunk = RenderEngine::getInstance()->assignChunk(renderer);

				// Only remove the renderer from this chunk if another chunk is found.
				// If not, keep the renderer here until a new chunk is found.
				if (newChunk && newChunk != this)
				{
					newChunk->addRenderer(renderer);
					removeFromList = true;
				}
			}
		}

		if (renderer->getIsPendingToBeDestroyed())
		{
			renderer->finallyDestroy();
			RenderEngine::getInstance()->freeRenderer(renderer);
		}

		if (renderer->getIsDestroyed())
		{
			removeFromList = true;
		}

		if (removeFromList)
		{
			it = mRenderers->erase(it);
			--it;
		}
	}
}

void Chunk::load()
{
	if (!mIsLoaded)
	{
		mIsLoaded = true; /*ECHO("load")*/
	}
}
void Chunk::unload()
{
	if (mIsLoaded)
	{
		mIsLoaded = false; /*ECHO("unload")*/
	}
}

void Chunk::addRenderer(Renderer *renderer)
{
	mRenderers->push_back(renderer);
}

bool Chunk::containsRenderer(CNS Renderer *renderer, f32 epsilon /*= 0.0f*/) CNS
{
	Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
	bool contains = Geometry::testRectanglePoint(mLeftTop, mSize, mSize, rendererPosition, epsilon);
	return contains; // TODO : move to settings ?
}

bool Chunk::containsRendererSphere(CNS Renderer *renderer) CNS
{
	Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
	return Geometry::testSphereSphere(mCenter, rendererPosition, mRadius,
									  renderer->getGameObject()->getTransform()->getScale().y * 2.0f, 0);
}