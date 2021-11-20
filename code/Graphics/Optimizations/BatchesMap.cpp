#include "Graphics/Optimizations/BatchesMap.hpp"

#include "Graphics/Optimizations/Batch.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Log/Log.hpp"
#include "Profiler/Profiler.hpp"

BatchesMap::~BatchesMap()
{
	MAP_DELETE_CONTENT(mBatchesDynamic)
	MAP_DELETE_CONTENT(mBatchesDynamicScreenSpace)
	MAP_DELETE_CONTENT(mBatchesStatic)
	MAP_DELETE_CONTENT(mBatchesStaticScreenSpace)
}

void BatchesMap::init()
{
	//TRACE();
}

void BatchesMap::addRenderer(Renderer *renderer)
{
	Texture *texture = renderer->getMaterial()->getTexture(); // NOTE : Texture can be nullptr as a valid hash key.

	Transform* transform = renderer->getGameObject()->getTransform();

	std::map<Texture *, Batch *>* batchesMap = nullptr;
	
	if(transform->isStatic())
	{
		batchesMap = &(transform->getAffectedByProjection() ?  mBatchesStatic : mBatchesStaticScreenSpace);
	}
	else
	{
		batchesMap = &(transform->getAffectedByProjection() ?  mBatchesDynamic : mBatchesDynamicScreenSpace);
	}

	if (!MAP_CONTAINS(*batchesMap, texture))
	{
		Batch *batch = NEW(Batch);
		batch->init(renderer->getMesh(), renderer->getMaterial());
		// batch->setChunk(chunk);
		batch->setIsStatic(transform->isStatic());
		batch->setIsWorldSpace(transform->getAffectedByProjection());

		MAP_INSERT(*batchesMap, texture, batch);
	}

	(*batchesMap).at(texture)->addRenderer(renderer);
}

void BatchesMap::render(u32 layer)
{
	renderBatchesMap(layer, mBatchesStatic);
	renderBatchesMap(layer, mBatchesDynamic);
	renderBatchesMap(layer, mBatchesStaticScreenSpace);
	renderBatchesMap(layer, mBatchesDynamicScreenSpace);
}

void BatchesMap::renderBatchesMap(u32 layer, std::map<Texture *, Batch *>& batchesMap)
{
	FOR_MAP(it, batchesMap)
	{
		it->second->render(layer);
	}
}