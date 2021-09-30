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
	PRI(RenderEngine *, RenderEngine, NONE)
	PRI(SMap(u32, SLst(Renderer *) *), Renderers, NONE)
	PRI(Material *, Material, GET)
	PRI(CNS Mesh *, Mesh, GET)

	PRI(Mesh, MeshBuilder, NONE)

	PRI(u32, VBOPosition, NONE) // TODO: change u32 for GLuint
	PRI(u32, EBO, NONE)
	PRI(u32, VBOTexture, NONE)
	PRI(u32, VBOColor, NONE)
	PRI(u32, VBONormal, NONE)
	PRI(u32, VAO, NONE)

	PRI(u32, MaxMeshesThreshold, NONE)
	PRI(u32, MaxMeshesIncrement, NONE)
	PRI(u32, MeshesIndex, NONE)

	PRI(bool, Binded, NONE)
	PRI(bool, IsWorldSpace, GET_SET)

private:
	void addToVertexBuffer(Renderer * renderer);
	void resizeVertexBuffers(u32 newSize);
	bool isChunkOk(Renderer * renderer) CNS;

	void processRenderers(SLst(Renderer *) * renderers);

	void drawCall() CNS;

	void internalRemoveRendererFromList(SLst(Renderer *)::iterator & it, SLst(Renderer *) * list);

	void insertSorted(Renderer * renderer, SLst(Renderer *) * renderers);

public:
	~Batch() OVR;

	void init(CNS Mesh *mesh, Material *material);
	void bind();

	void render(u32 layer);

	void addRenderer(Renderer * renderer);
};