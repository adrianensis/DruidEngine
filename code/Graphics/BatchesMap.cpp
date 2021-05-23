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
}

void BatchesMap::addRenderer(Renderer *renderer) {

	Texture* texture = renderer->getMaterial()->getTexture(); // NOTE : Texture can be nullptr as a valid hash key.

	if (! MAP_CONTAINS(mBatches, texture)) {

		Batch* batch = new Batch;
		batch->init(renderer->getMesh(), renderer->getMaterial());
		// batch->setChunk(chunk);

		MAP_INSERT(mBatches, texture, batch);
	}

	mBatches.at(texture)->addRenderer(renderer);
}

u32 BatchesMap::render(u32 layer) {
	u32 drawCallCounter = 0;

	FOR_MAP(it, mBatches) {
		drawCallCounter += it->second->render(layer);
	}

	return drawCallCounter;
}