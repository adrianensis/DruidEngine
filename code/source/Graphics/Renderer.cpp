#include "Renderer.h"

#include "Mesh.h"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component() {
  mMesh = nullptr;
  mMaterial = nullptr;
}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

// ---------------------------------------------------------------------------

void Renderer::init() { }

// ---------------------------------------------------------------------------

void Renderer::setMesh(Mesh* mesh){ mMesh = mesh; }
Mesh* Renderer::getMesh(){ return mMesh; }
void Renderer::setMaterial(Material* material){ mMaterial = material; }
Material* Renderer::getMaterial(){ return mMaterial; }

// ---------------------------------------------------------------------------

} /* namespace DE */
