#include "Engine.h"
#include "RenderEngine.h"

namespace DE {

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::init(){
  Memory::init();
};

void Engine::run(){

	RenderEngine render;

	render.init();

	while(! render.isClosed()){
		render.step();
	}

	render.terminate();
};

void Engine::terminate() {
	Memory::free();
}

} /* namespace DE */
