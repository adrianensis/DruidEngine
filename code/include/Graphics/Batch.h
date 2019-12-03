#ifndef DE_BATCH_H
#define DE_BATCH_H

#include "DE_Class.h"

namespace DE {

class Material;
class Mesh;
class Renderer;
class RenderEngine;
template <class T> class List;

class Batch : public DE_Class{

private:

	RenderEngine* mRenderEngine;

	List<Renderer*>* mRenderers;
	Material* mMaterial;
	Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mEBO;
	u32 mVBOTexture;
	//u32 mVBOColor;
	u32 mVBONormal;
	u32 mVAO;

	u32 mTextureId;

public:

	DE_CLASS(Batch, DE_Class);

	void init(RenderEngine* renderEngine, Mesh* mesh, Material* material);
	void bind();
	void update();
	void render(u32 layer);

	void addRenderer(Renderer* renderer);

	DE_SET(Mesh*, Mesh);
	DE_SET(Material*, Material);
};

} /* namespace DE */

#endif /* DE_BATCH_H */
