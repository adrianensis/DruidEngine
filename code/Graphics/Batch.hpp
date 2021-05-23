#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Matrix4.hpp"

#include <vector>
#include <map>

class Material;
class Mesh;
class Renderer;
class RenderEngine;
class Camera;

CLASS(Batch, ObjectBase) {

	PRI(RenderEngine, NONE, RenderEngine*)
	PRI(Renderers, NONE, std::map<u32, std::list<Renderer*>*>)
	PRI(Material, GET, Material*)
	PRI(Mesh, GET, const Mesh*)

	PRI(VBOPosition, NONE, u32) // TODO: change u32 for GLuint
	PRI(EBO, NONE, u32)
	PRI(VBOTexture, NONE, u32)
	PRI(VBOColor, NONE, u32)
	PRI(VBONormal, NONE, u32)
	PRI(VAO, NONE, u32)

	PRI(MaxVertexBufferSize, NONE, u32)
	PRI(MaxMeshes, NONE, u32)
	PRI(MeshesIndex, NONE, u32)
	PRI(VerticesPerMesh, NONE, u32)
	PRI(VertexPositionSize, NONE, u32)
	PRI(VertexTextureSize, NONE, u32)
	PRI(VertexColorSize, NONE, u32)
	PRI(FacesSize, NONE, u32)
	PRI(PositionBufferIndex, NONE, u32)
	PRI(TextureBufferIndex, NONE, u32)
	PRI(ColorBufferIndex, NONE, u32)
	PRI(PositionBuffer, NONE, std::vector<f32>);
	PRI(TextureBuffer, NONE, std::vector<f32>);
	PRI(ColorBuffer, NONE, std::vector<f32>);
	PRI(IndicesBuffer, NONE, std::vector<u32>);

	PRI(Binded, NONE, bool)
	PRI(TextureId, NONE, u32)

	void addToVertexBuffer(Renderer* renderer);
	void clearVertexBuffer();

	bool checkInFrustum(Camera *cam, Renderer *renderer);
	bool checkDistance(Camera *cam, Renderer *renderer);
	bool checkIsOutOfCamera(Camera *cam, Renderer *renderer);

	void internalRemoveRendererFromList(std::list<Renderer*>::iterator &it, std::list<Renderer*> *list);

	static u8 rendererYCoordinateComparator(Renderer *a, Renderer *b);

	void sort(u32 layer);
	void insertSorted(Renderer *renderer, std::list<Renderer*> *renderers);

	static bool smIsScreenOrthoReady;
	static Matrix4 smScreenOrtho;

public:

	Batch();
	~Batch() override;

	void init(const Mesh *mesh, Material *material);
	void bind();
	void update();

	// returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(class Renderer *renderer);
};