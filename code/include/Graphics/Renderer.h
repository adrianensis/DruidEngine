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

	DE_GENERATE_METADATA(Renderer)

	Renderer();
	~Renderer() override;

	virtual void init() override;

	void setMesh(Mesh* mesh);
	Mesh* getMesh();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
