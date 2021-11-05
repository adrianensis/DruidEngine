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
	PRI std::map<u32, std::list<Renderer *> *> mRenderers = {};
	PRI Material * mMaterial = {}; GET(Material)
	PRI const Mesh * mMesh = {}; GET(Mesh)

	PRI Mesh mMeshBuilder = {};

	PRI u32 mVBOPosition = {}; // TODO: change u32 for GLuint
	PRI u32 mEBO = {};
	PRI u32 mVBOTexture = {};
	PRI u32 mVBOColor = {};
	PRI u32 mVBONormal = {};
	PRI u32 mVAO = {};

	PRI u32 mMaxMeshesThreshold = {};
	PRI u32 mMaxMeshesIncrement = {};
	PRI u32 mMeshesIndex = {};

	PRI bool mBinded = {};
	PRI bool mIsWorldSpace = {}; GET_SET(IsWorldSpace)

PRI
	void addToVertexBuffer(Renderer * renderer);
	void resizeVertexBuffers(u32 newSize);
	bool isChunkOk(Renderer * renderer) const;

	void processRenderers(std::list<Renderer *> * renderers);

	void drawCall() const;

	void internalRemoveRendererFromList(std::list<Renderer *>::iterator & it, std::list<Renderer *> * list);

	void insertSorted(Renderer * renderer, std::list<Renderer *> * renderers);

PUB
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();

	void render(u32 layer);

	void addRenderer(Renderer * renderer);
};