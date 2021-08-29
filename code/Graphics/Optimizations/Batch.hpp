#pragma once

#include "Core/Core.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Mesh.hpp"

#include <vector>
#include <map>
#include <list>

class Material;
class Renderer;
class RenderEngine;
class Camera;

CLASS(Batch, ObjectBase)
{
	PRI(RenderEngine, NONE, RenderEngine *)
	PRI(Renderers, NONE, std::map<u32, std::list<Renderer *> *>)
	PRI(Material, GET, Material *)
	PRI(Mesh, GET, const Mesh *)

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
	bool isChunkOk(Renderer * renderer) const;

	void processRenderers(std::list<Renderer *> * renderers);

	void drawCall() const;

	void internalRemoveRendererFromList(std::list<Renderer *>::iterator & it, std::list<Renderer *> * list);

	void insertSorted(Renderer * renderer, std::list<Renderer *> * renderers);

public:
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();

	void render(u32 layer);

	void addRenderer(Renderer * renderer);
};