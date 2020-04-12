#ifndef DE_BATCH_H
#define DE_BATCH_H

#include "DE_Class.h"
#include "Matrix4.h"

namespace DE {

class Material;
class Mesh;
class Renderer;
class RenderEngine;
class Camera;
template <class T> class List;

class Iterator;

class Batch : public DE_Class{

private:

	RenderEngine* mRenderEngine;

	List<Renderer*>* mRenderers;

	Material* mMaterial;
	const Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mEBO;
	u32 mVBOTexture;
	u32 mVBONormal;
	u32 mVAO;

	bool mBinded;

	u32 mTextureId;

	static Matrix4 smMatrixIdentity;

	bool checkInFrustum(Camera* cam, Renderer* renderer);
	bool checkDistance(Camera* cam, Renderer* renderer);
	bool checkOutOfCamera(Camera* cam, Renderer* renderer);

	void internalRemoveRenderer(const Iterator* it);

public:

	DE_CLASS(Batch, DE_Class);

	void init(const Mesh* mesh, Material* material);
	void bind();
	void update();

	// it returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(Renderer* renderer);

	void setMesh(const Mesh* newMesh ){ mMesh = newMesh; };
	void setMaterial(Material* newMaterial ){ mMaterial = newMaterial; };
};

} /* namespace DE */

#endif /* DE_BATCH_H */
