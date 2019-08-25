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
	u32 mVBOColor;
	u32 mVBONormal;
	u32 mVAO;

	u32 mTextureId;

public:

	DE_CLASS(Batch, DE_Class);

	void init(RenderEngine* renderEngine, Mesh* mesh, Material* material);
	void bind();
	void update();
	void render();

	void setMesh(Mesh* mesh);
	void setMaterial(Material* material);
	void addRenderer(Renderer* renderer);
};

} /* namespace DE */

#endif /* DE_BATCH_H */
