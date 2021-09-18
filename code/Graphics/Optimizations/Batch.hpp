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
	PRI(RenderEngine, NONE, RenderEngine *)
	PRI(Renderers, NONE, SMap<u32, SLst<Renderer *> *>)
	PRI(Material, GET, Material *)
	PRI(Mesh, GET, CNS Mesh *)

	PRI(MeshBuilder, NONE, Mesh)

	PRI(VBOPosition, NONE, u32) // TODO: change u32 for GLuint
	PRI(EBO, NONE, u32)
	PRI(VBOTexture, NONE, u32)
	PRI(VBOColor, NONE, u32)
	PRI(VBONormal, NONE, u32)
	PRI(VAO, NONE, u32)

	PRI(MaxMeshesThreshold, NONE, u32)
	PRI(MaxMeshesIncrement, NONE, u32)
	PRI(MeshesIndex, NONE, u32)

	PRI(Binded, NONE, bool)
	PRI(IsWorldSpace, GET_SET, bool)

private:
	void addToVertexBuffer(Renderer * renderer);
	void resizeVertexBuffers(u32 newSize);
	bool isChunkOk(Renderer * renderer) CNS;

	void processRenderers(SLst<Renderer *> * renderers);

	void drawCall() CNS;

	void internalRemoveRendererFromList(SLst<Renderer *>::iterator & it, SLst<Renderer *> * list);

	void insertSorted(Renderer * renderer, SLst<Renderer *> * renderers);

public:
	~Batch() OVR;

	void init(CNS Mesh *mesh, Material *material);
	void bind();

	void render(u32 layer);

	void addRenderer(Renderer * renderer);
};