#include "Graphics/Optimizations/BatchesMap.hpp"

#include "Graphics/Optimizations/Batch.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Scenes/Scenes.hpp"

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
	// Create a temporary key for searching purposes
	BatchKey tmpBatchKey;
	tmpBatchKey.init(
		renderer->getMaterial()->getTexture(), // NOTE : Texture can be nullptr as a valid hash key.
		renderer->getMaterial()->getShader(),
		renderer->getMesh()
	);

	// Find if batch key already exists
	BatchKey* foundBatchKey = nullptr;
	FOR_LIST(itBatchKey, mBatchKeys)
	{
		BatchKey* batchKey = &(*itBatchKey);
		foundBatchKey = (*batchKey) == tmpBatchKey ? batchKey : nullptr; 
	}

	// If not found, register the key
	if(!foundBatchKey)
	{
		foundBatchKey = &(mBatchKeys.emplace_back(tmpBatchKey));
		foundBatchKey->init(
			renderer->getMaterial()->getTexture(), // NOTE : Texture can be nullptr as a valid hash key.
			renderer->getMaterial()->getShader(),
			renderer->getMesh()
		);
	}

	Transform* transform = renderer->getGameObject()->getTransform();

	std::map<BatchKey*, Batch*>* batchesMap = nullptr;
	
	if(transform->isStatic())
	{
		batchesMap = &(transform->getAffectedByProjection() ?  mBatchesStatic : mBatchesStaticScreenSpace);
	}
	else
	{
		batchesMap = &(transform->getAffectedByProjection() ?  mBatchesDynamic : mBatchesDynamicScreenSpace);
	}

	if (!MAP_CONTAINS(*batchesMap, foundBatchKey))
	{
		Batch *batch = NEW(Batch);
		batch->init(renderer->getMesh(), renderer->getMaterial());
		batch->setIsStatic(transform->isStatic());
		batch->setIsWorldSpace(transform->getAffectedByProjection());

		MAP_INSERT(*batchesMap, foundBatchKey, batch);
	}

	(*batchesMap).at(foundBatchKey)->addRenderer(renderer);
}

void BatchesMap::render()
{
	renderBatchesMap(mBatchesStatic);
	renderBatchesMap(mBatchesDynamic);
	renderBatchesMap(mBatchesStaticScreenSpace);
	renderBatchesMap(mBatchesDynamicScreenSpace);
}

void BatchesMap::renderBatchesMap(std::map<BatchKey*, Batch*>& batchesMap)
{
	FOR_MAP(it, batchesMap)
	{
		it->second->render();
	}
}

void BatchesMap::forceRegenerateBuffers()
{
	FOR_MAP(it, mBatchesStatic)
	{
		it->second->forceRegenerateBuffers();
	}
}