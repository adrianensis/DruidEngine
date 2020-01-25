#ifndef DE_BATCHSTATIC_H
#define DE_BATCHSTATIC_H

#include "DE_Class.h"
#include "Vector2.h"

namespace DE {

class Material;
class Mesh;
class Renderer;
class RenderEngine;
class Camera;
template <class T> class List;

class BatchMap : public DE_Class{

private:

	RenderEngine* mRenderEngine;

	Material* mMaterial;
	Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mEBO;
	u32 mVBOTexture;
	u32 mVBONormal;
	u32 mVAO;

	u32 mTextureId;

	u32 mTilesCount;

	Vector2 mMapSize;

public:

	DE_CLASS(BatchMap, DE_Class);

	void init(RenderEngine* renderEngine, Material* material, const Vector2& mapSize) ;
	void bind();
	void update();

	// it returns the count of draw calls
	u32 render();

	void addRenderer(Renderer* renderer);

	DE_SET(Mesh*, Mesh);
	DE_SET(Material*, Material);
};

} /* namespace DE */

#endif /* DE_BATCHSTATIC_H */
