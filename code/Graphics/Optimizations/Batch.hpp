#pragma once

#include "Core/Core.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Mesh.hpp"


class Material;
class Renderer;
class RenderEngine;
class Camera;

class Batch: public ObjectBase
{
    GENERATE_METADATA(Batch)

	PRI std::list<Renderer *> mRenderers;

	PRI Material* mMaterial = nullptr; GET(Material)
	PRI const Mesh* mMesh = nullptr; GET(Mesh)

	PRI Mesh mMeshBuilder;

	PRI u32 mVBOPosition = 0; // TODO: change u32 for GLuint
	PRI u32 mEBO = 0;
	PRI u32 mVBOTexture = 0;
	PRI u32 mVBOColor = 0;
	PRI u32 mVBONormal = 0;
	PRI u32 mVAO = 0;

	PRI u32 mMaxMeshesThreshold = 0;
	PRI u32 mMaxMeshesIncrement = 0;
	PRI u32 mMeshesIndex = 0;

	PRI bool mBinded = false;
	PRI bool mIsWorldSpace = false; GET_SET(IsWorldSpace)
	PRI bool mIsStatic = false; GET_SET(IsStatic)

	PUB bool mNewRendererAdded = false;
	PUB bool mPendingDrawCall = false;

PRI
	bool shouldRegenerateBuffers() const;

	void addToVertexBuffer(Renderer * renderer);
	void resizeVertexBuffers(u32 newSize);
	bool isChunkOk(Renderer * renderer) const;

	bool processRenderers(std::list<Renderer *>& renderers);

	void drawCall();

	void internalRemoveRendererFromList(std::list<Renderer *>::iterator & it, std::list<Renderer *>& list);

	void insertSorted(Renderer * renderer, std::list<Renderer *> * renderers);

PUB
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();

	void render();

	void addRenderer(Renderer * renderer);
};