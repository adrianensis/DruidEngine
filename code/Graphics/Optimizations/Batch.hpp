#pragma once

#include "Core/Core.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Mesh.hpp"


class Material;
class Renderer;
class RenderEngine;
class Camera;

CLASS(Batch, ObjectBase)
{
	PRI_M(RenderEngine *, RenderEngine, NONE)
	PRI_M(SMap(u32, SLst(Renderer *) *), Renderers, NONE)
	PRI_M(Material *, Material, GET)
	PRI_M(const Mesh *, Mesh, GET)

	PRI_M(Mesh, MeshBuilder, NONE)

	PRI_M(u32, VBOPosition, NONE) // TODO: change u32 for GLuint
	PRI_M(u32, EBO, NONE)
	PRI_M(u32, VBOTexture, NONE)
	PRI_M(u32, VBOColor, NONE)
	PRI_M(u32, VBONormal, NONE)
	PRI_M(u32, VAO, NONE)

	PRI_M(u32, MaxMeshesThreshold, NONE)
	PRI_M(u32, MaxMeshesIncrement, NONE)
	PRI_M(u32, MeshesIndex, NONE)

	PRI_M(bool, Binded, NONE)
	PRI_M(bool, IsWorldSpace, GET_SET)

PRI
	void addToVertexBuffer(Renderer * renderer);
	void resizeVertexBuffers(u32 newSize);
	bool isChunkOk(Renderer * renderer) const;

	void processRenderers(SLst(Renderer *) * renderers);

	void drawCall() const;

	void internalRemoveRendererFromList(SLst(Renderer *)::iterator & it, SLst(Renderer *) * list);

	void insertSorted(Renderer * renderer, SLst(Renderer *) * renderers);

PUB
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();

	void render(u32 layer);

	void addRenderer(Renderer * renderer);
};