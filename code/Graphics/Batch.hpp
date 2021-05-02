#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Matrix4.hpp"
#include "Containers/List.hpp"
#include "Containers/Array.hpp"
#include "Containers/HashMap.hpp"

namespace DE {

class Batch: public ObjectBase {

private:

	 class RenderEngine* mRenderEngine;
	 HashMap<u32, List<class Renderer*>*>* mRenderers;
	 class Material* mMaterial;
	 const class Mesh* mMesh;

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
	 Array<u32>* mIndicesBuffer;

	 bool mBinded;
	 u32 mTextureId;

	void addToVertexBuffer(class Renderer* renderer);
	void clearVertexBuffer();

	bool checkInFrustum(class Camera *cam, class Renderer *renderer);
	bool checkDistance(class Camera *cam, class Renderer *renderer);
	bool checkIsOutOfCamera(class Camera *cam, class Renderer *renderer);

	void internalRemoveRendererFromList(const class Iterator *it, List<class Renderer*> *list);

	static u8 rendererYCoordinateComparator(class Renderer *a, class Renderer *b);

	void sort(u32 layer);
	void insertSorted(class Renderer *renderer, List<class Renderer*> *renderers);

	static bool smIsScreenOrthoReady;
	static Matrix4 smScreenOrtho;

public:

	GENERATE_METADATA(Batch);

	Batch();
	virtual ~Batch() override;;

	SET(Material);
	SET(Mesh);

	void init(const class Mesh *mesh, class Material *material);
	void bind();
	void update();

	// returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(class Renderer *renderer);
};
}

