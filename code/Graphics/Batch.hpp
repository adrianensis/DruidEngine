#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Mesh.hpp"

#include <vector>
#include <map>

class Material;
class Renderer;
class RenderEngine;
class Camera;

CLASS(Batch, ObjectBase) {

	PRI(RenderEngine, NONE, RenderEngine*)
	PRI(Renderers, NONE, std::map<u32, std::list<Renderer*>*>)
	PRI(Material, GET, Material*)
	PRI(Mesh, GET, const Mesh*)

	PRI(MeshBuilder, NONE, Mesh)

	PRI(VBOPosition, NONE, u32) // TODO: change u32 for GLuint
	PRI(EBO, NONE, u32)
	PRI(VBOTexture, NONE, u32)
	PRI(VBOColor, NONE, u32)
	PRI(VBONormal, NONE, u32)
	PRI(VAO, NONE, u32)

	PRI(MaxMeshes, NONE, u32)
	PRI(MeshesIndex, NONE, u32)

	PRI(Binded, NONE, bool)
	PRI(IsWorldSpace, GET_SET, bool)

	void addToVertexBuffer(Renderer* renderer);
	void clearVertexBuffer();
	bool isChunkOk(Renderer* renderer) const;

	void processRenderers(std::list<Renderer*>* renderers);

	void drawCall() const;

	void internalRemoveRendererFromList(std::list<Renderer*>::iterator &it, std::list<Renderer*> *list);

	void insertSorted(Renderer *renderer, std::list<Renderer*> *renderers);

public:

	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();
	void update();

	// returns the count of draw calls
	void render(u32 layer);

	void addRenderer(class Renderer *renderer);
};