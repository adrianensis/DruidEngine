#pragma once

#include "Core/Core.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Renderer.hpp"

class Material;
class RenderEngine;
class Camera;

class Batch: public ObjectBase
{
    GENERATE_METADATA(Batch)

	PRI std::list<ProxyObject<Renderer>> mProxyRenderers;

	PRI Material* mMaterial = nullptr; GET(Material)
	PRI const Mesh* mMesh = nullptr; GET(Mesh)
	PRI bool mIsInstanced = false; GET(IsInstanced)
	PRI std::vector<f32> mMatrices;

	PRI Mesh mMeshBuilder;

	PRI u32 mVAO = 0;
	PRI u32 mVBOPosition = 0; // TODO: change u32 for GLuint
	PRI u32 mVBOTexture = 0;
	PRI u32 mVBOColor = 0;
	PRI u32 mVBOMatrices = 0;
	PRI u32 mEBO = 0;

	PRI u32 mMaxMeshesThreshold = 0;
	PRI const u32 mMaxMeshesIncrement = 100;
	PRI u32 mMeshesIndex = 0;

	PRI bool mBinded = false;
	PRI bool mDataSentToGPU = false;
	PRI bool mIsWorldSpace = false; GET_SET(IsWorldSpace)
	PRI bool mIsStatic = false; GET_SET(IsStatic)

	PRI bool mNewRendererAdded = false;
	PRI bool mPendingDrawCall = false;
	PRI bool mForceRegenerateBuffers = false;

PRI
	bool shouldRegenerateBuffers() const;

	void addToVertexBuffer(Renderer * renderer);

	void generateFacesData(u32 meshesCount);
	void invalidateAndReallocateBuffers();
	void sendDataToBuffers();

	void addToVertexBufferNotInstanced(Renderer * renderer);
	void addToVertexBufferInstanced(Renderer * renderer);

	void resizeBuffers();
	bool isChunkOk(Renderer * renderer) const;

	bool processRenderers();

	void drawCall();

	void internalRemoveRendererFromList(std::list<ProxyObject<Renderer>>::iterator & it);
PUB
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();

	void render();

	void addRenderer(Renderer * renderer);
	void forceRegenerateBuffers() { mForceRegenerateBuffers = true; }
};