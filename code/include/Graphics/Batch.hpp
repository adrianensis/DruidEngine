#ifndef DE_BATCH_H
#define DE_BATCH_H

#include "DE_Class.hpp"
#include "Matrix4.hpp"

namespace DE {

class Material;
class Mesh;
class Renderer;
class RenderEngine;
class Camera;
class Chunk;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;

class Iterator;

class Batch: public DE_Class {

private:

	RenderEngine* mRenderEngine;

	HashMap<u32, List<Renderer*>*>* mRenderers;
	HashMap<u32, List<Renderer*>*>* mDynamicRenderers;

	Material* mMaterial;
	const Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mEBO;
	u32 mVBOTexture;
	u32 mVBOColor;
	u32 mVBONormal;
	u32 mVAO;

	u32 mMaxVertexBufferSize;
	u32 mMaxMeshes;
	u32 mMeshesIndex;
	u32 mVerticesPerMesh;
	u32 mVertexPositionSize;
	u32 mVertexTextureSize;
	u32 mVertexColorSize;
	u32 mFacesSize;
	u32 mPositionBufferIndex;
	u32 mTextureBufferIndex;
	u32 mColorBufferIndex;
	Array<f32>* mPositionBuffer;
	Array<f32>* mTextureBuffer;
	Array<f32>* mColorBuffer;
	Array<u32>* mFacesBuffer;

	void addToVertexBuffer(Renderer* renderer);
	void clearVertexBuffer();

	bool mBinded;

	u32 mTextureId;

	bool checkInFrustum(Camera *cam, Renderer *renderer);
	bool checkDistance(Camera *cam, Renderer *renderer);
	bool checkOutOfCamera(Camera *cam, Renderer *renderer);

	void internalRemoveRenderer(const Iterator *it, List<Renderer*> *list);
	void internalRemoveRendererFromList(const Iterator *it, List<Renderer*> *list);

	static u8 rendererYCoordinateComparator(Renderer *a, Renderer *b);

	void sort(u32 layer);
	void insertSorted(Renderer *renderer, List<Renderer*> *renderers);

	static bool smIsScreenOrthoReady;
	static Matrix4 smScreenOrtho;

public:

	DE_CLASS(Batch, DE_Class);

	void init(const Mesh *mesh, Material *material);
	void bind();
	void update();

	// it returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(Renderer *renderer);

	void setMesh(const Mesh *newMesh) {
		mMesh = newMesh;
	};

	void setMaterial(Material *newMaterial) {
		mMaterial = newMaterial;
	};
};

} /* namespace DE */

#endif /* DE_BATCH_H */
