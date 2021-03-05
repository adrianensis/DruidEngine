#pragma once

#include "Core/DE_Class.hpp"
#include "Maths/Matrix4.hpp"

namespace DE {

class Material;
class Mesh;
class Renderer;
class RenderEngine;
class Camera;
class Chunk;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;

class Iterator;

class Batch: public DE_Class {

private:

	DE_M(RenderEngine, RenderEngine*)

	using LayersRenderersMap = HashMap<u32, List<Renderer*>*>;
	DE_M(Renderers, LayersRenderersMap*)

	DE_M_SET(Material, Material*)
	DE_M_SET(Mesh, const Mesh*)

	DE_M(VBOPosition, u32); // TODO: change u32 for GLuint
	DE_M(EBO, u32);
	DE_M(VBOTexture, u32);
	DE_M(VBOColor, u32);
	DE_M(VBONormal, u32);
	DE_M(VAO, u32);

	DE_M(MaxVertexBufferSize, u32);
	DE_M(MaxMeshes, u32);
	DE_M(MeshesIndex, u32);
	DE_M(VerticesPerMesh, u32);
	DE_M(VertexPositionSize, u32);
	DE_M(VertexTextureSize, u32);
	DE_M(VertexColorSize, u32);
	DE_M(FacesSize, u32);
	DE_M(PositionBufferIndex, u32);
	DE_M(TextureBufferIndex, u32);
	DE_M(ColorBufferIndex, u32);
	DE_M(PositionBuffer, Array<f32>*);
	DE_M(TextureBuffer, Array<f32>*);
	DE_M(ColorBuffer, Array<f32>*);
	DE_M(FacesBuffer, Array<u32>*);

	DE_M(Binded, bool);
	DE_M(TextureId, u32);

	void addToVertexBuffer(Renderer* renderer);
	void clearVertexBuffer();

	bool checkInFrustum(Camera *cam, Renderer *renderer);
	bool checkDistance(Camera *cam, Renderer *renderer);
	bool checkIsOutOfCamera(Camera *cam, Renderer *renderer);

	void internalRemoveRenderer(const Iterator *it, List<Renderer*> *list);
	void internalRemoveRendererFromList(const Iterator *it, List<Renderer*> *list);

	static u8 rendererYCoordinateComparator(Renderer *a, Renderer *b);

	void sort(u32 layer);
	void insertSorted(Renderer *renderer, List<Renderer*> *renderers);

	static bool smIsScreenOrthoReady;
	static Matrix4 smScreenOrtho;

public:

	DE_CLASS_BODY(Batch)

	void init(const Mesh *mesh, Material *material);
	void bind();
	void update();

	// it returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(Renderer *renderer);
};

}

