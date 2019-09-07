#include "RenderEngine.h"
#include "Memory.h"
#include "Batch.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Renderer.h"
#include "List.h"
#include "HashMap.h"
#include "Debug.h"

namespace DE {

// ---------------------------------------------------------------------------

RenderEngine::RenderEngine() : DE_Class() {
	mBatches = nullptr;
	mCamera = nullptr;
}

RenderEngine::~RenderEngine() = default;

// ---------------------------------------------------------------------------

void RenderEngine::init() {

	mBatches = Memory::allocate<HashMap<Texture*, Batch*>>();

	RenderContext::init();
	mBatches->init();
}

// ---------------------------------------------------------------------------

void RenderEngine::step() {

	u32 i=0;
	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		//ECHO("BATCH NUM")
		//VAL(u32,i+1)
		it.get()->render();
	}

	RenderContext::swap();
}

// ---------------------------------------------------------------------------

void RenderEngine::bind() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		it.get()->bind();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::update() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		it.get()->update();
	}
}

// ---------------------------------------------------------------------------

void RenderEngine::terminate() {

	for (auto it = mBatches->getValues()->getIterator(); !it.isNull(); it.next()){
		Memory::free<Batch>(it.get());
	}

	Memory::free<HashMap<Texture*, Batch*>>(mBatches);

	RenderContext::terminate();
}

// ---------------------------------------------------------------------------

void RenderEngine::addRenderer(Renderer* renderer) {

	Texture* texture = renderer->getMaterial()->getTexture();

	if( ! mBatches->contains(texture)) {

		Batch* batch = Memory::allocate<Batch>();
		batch->init(this, renderer->getMesh(), renderer->getMaterial());

		mBatches->set(texture, batch);
	}

	mBatches->get(texture)->addRenderer(renderer);
}

// ---------------------------------------------------------------------------

void RenderEngine::setCamera(Camera* camera){
	mCamera = camera;
}

// ---------------------------------------------------------------------------

Camera* RenderEngine::getCamera() const {
 	return mCamera;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
