#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.h"

namespace DE {

class Material;
class Mesh;

class Renderer : public Component{

private:

	Material* mMaterial;
	Mesh* mMesh;

public:

	DE_CLASS(Renderer);

	virtual void init() override;

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setMaterial(Material* material);
	Material* getMaterial();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
