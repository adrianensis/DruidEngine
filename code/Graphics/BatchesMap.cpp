#include "Graphics/BatchesMap.hpp"

#include "Graphics/Batch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Log/Log.hpp"

BatchesMap::~BatchesMap() {
	FOR_MAP(it, mBatches) {
		delete it->second;
	}
}

void BatchesMap::init() {
	//TRACE();

	mIsWorldSpace = true;
}

void BatchesMap::addRenderer(Renderer *renderer) {

	Texture* texture = renderer->getMaterial()->getTexture(); // NOTE : Texture can be nullptr as a valid hash key.

	if (! MAP_CONTAINS(mBatches, texture)) {

		Batch* batch = new Batch;
		batch->init(renderer->getMesh(), renderer->getMaterial());
		// batch->setChunk(chunk);
		batch->setIsWorldSpace(mIsWorldSpace);

		MAP_INSERT(mBatches, texture, batch);
	}

	mBatches.at(texture)->addRenderer(renderer);
}

void BatchesMap::render(u32 layer) {
	FOR_MAP(it, mBatches) {
		it->second->render(layer);
	}
}

void BatchesMap::setIsWorldSpace(bool isWorldSpace) {
	mIsWorldSpace = isWorldSpace;
	FOR_MAP(it, mBatches) {
		it->second->setIsWorldSpace(mIsWorldSpace);
	}
}