#pragma once

#include "Core/Core.hpp"

class Vector3;


CLASS(Mesh, ObjectBase)
{
	PRI(SVec(f32), Vertices, GETREF_CONST)
	PRI(SVec(f32), Normals, GETREF_CONST)
	PRI(SVec(f32), TextureCoordinates, GETREF_CONST)
	PRI(SVec(f32), Colors, GETREF_CONST)
	PRI(SVec(u32), Faces, GETREF_CONST)

	PRI(u32, VertexCount, GET)
	PRI(u32, FacesCount, GET)

private:
	static Mesh *smRectangle;

public:
	static u32 smVertexPositionSize;
	static u32 smVertexNormalSize;
	static u32 smVertexTexCoordSize;
	static u32 smVertexColorSize;
	static u32 smFaceSize;

	void init(u32 vertexCount, u32 facesCount);
	Mesh *addVertex(const Vector3 &vec);
	Mesh *addNormal(const Vector3 &vec);
	Mesh *addTexCoord(f32 u, f32 v);
	Mesh *addColor(f32 r, f32 g, f32 b, f32 a);
	Mesh *addFace(u32 v1, u32 v2, u32 v3);
	void clear();

	static Mesh *getRectangle();
	static void freeRectangle();
};