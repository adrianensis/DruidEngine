#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Core/ObjectBase.hpp"

#include <vector>

CLASS(Mesh, ObjectBase) {

	PRI(Vertices, GETREF_CONST, std::vector<f32>)
	PRI(Normals, GETREF_CONST, std::vector<f32>)
	PRI(TextureCoordinates, GETREF_CONST, std::vector<f32>)
	PRI(Colors, GETREF_CONST, std::vector<f32>)
	PRI(Faces, GETREF_CONST, std::vector<u32>)

	PRI(VertexCount, GET, u32)
	PRI(FacesCount, GET, u32)

private:
	static Mesh* smRectangle;

public:

	static u32 smVertexPositionSize;
	static u32 smVertexNormalSize;
	static u32 smVertexTexCoordSize;
	static u32 smVertexColorSize;
	static u32 smFaceSize;

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addColor(f32 r, f32 g, f32 b, f32 a);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);
	void clear();

	static Mesh* getRectangle();
	static void freeRectangle();
};