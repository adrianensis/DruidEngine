#ifndef DE_BATCH_H
#define DE_BATCH_H

#include "Material.h"
#include "Array.h"
#include "Renderer.h"
#include "Mesh.h"

namespace DE {

class Batch {
private:
	Material* mMaterial;
	Array<Renderer*>* mRenderers;
	Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mVBOElemIndices;
	u32 mVBOTexture;
	u32 mVBONormal;
	u32 mVAO;

public:
	Batch();
	~Batch();

	void bind();
};

} /* namespace DE */

#endif /* DE_BATCH_H */
