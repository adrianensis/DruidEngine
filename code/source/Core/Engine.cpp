#include "Engine.h"
#include "Memory.h"
#include "RenderContext.h"

namespace DE {

Engine::Engine() : DE_Class(){

};

Engine::~Engine() = default;

void Engine::init(){
  Memory::init();
  mRender.init();
};

void Engine::run(){
	mRender.bind();

	while(! RenderContext::isClosed()){
		mRender.update();
		mRender.render();
	}

	mRender.terminate();
};

void Engine::terminate() {
	Memory::free();
}

} /* namespace DE */
