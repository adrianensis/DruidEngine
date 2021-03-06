#include "Graphics/BatchesMap.hpp"

#include "Graphics/Batch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Containers/HashMap.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Log/Log.hpp"

namespace DE {

BatchesMap::BatchesMap() : DE_Class() {
	mBatches = nullptr;
}

BatchesMap::~BatchesMap() {
	FOR_LIST(it, mBatches->getValues()) {
		DE_FREE(it.get());
	}

	DE_FREE(mBatches);
}

void BatchesMap::init() {
	//TRACE();
	mBatches = DE_NEW<HashMap<Texture*, Batch*>>();
	mBatches->init();
}

void BatchesMap::addRenderer(Renderer *renderer) {

	Texture* texture = renderer->getMaterial()->getTexture(); // NOTE : Texture can be nullptr as a valid hash key.

	if (!mBatches->contains(texture)) {

		Batch* batch = DE_NEW<Batch>();
		batch->init(renderer->getMesh(), renderer->getMaterial());
		// batch->setChunk(chunk);

		mBatches->set(texture, batch);
	}

	mBatches->get(texture)->addRenderer(renderer);
}

u32 BatchesMap::render(u32 layer) {
	u32 drawCallCounter = 0;

	FOR_LIST(it, mBatches->getValues()) {
		drawCallCounter += it.get()->render(layer);
	}

	return drawCallCounter;
}

}
