#include "Renderer.h"

#include "Mesh.h"

namespace DE {

Renderer::Renderer() : Component() {
	// TODO Auto-generated constructor stub

}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

void Renderer::setMesh(Mesh* mesh){
  mMesh = mesh;
}

Mesh* Renderer::getMesh(){
  return mMesh;
}

} /* namespace DE */
