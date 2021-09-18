#pragma once

#include "Core/Core.hpp"

class Vector3;


CLASS(Mesh, ObjectBase)
{
	PRI(Vertices, GETREF_CONST, SVec<f32>)
	PRI(Normals, GETREF_CONST, SVec<f32>)
	PRI(TextureCoordinates, GETREF_CONST, SVec<f32>)
	PRI(Colors, GETREF_CONST, SVec<f32>)
	PRI(Faces, GETREF_CONST, SVec<u32>)

	PRI(VertexCount, GET, u32)
	PRI(FacesCount, GET, u32)

private:
	STC Mesh *smRectangle;

public:
	STC u32 smVertexPositionSize;
	STC u32 smVertexNormalSize;
	STC u32 smVertexTexCoordSize;
	STC u32 smVertexColorSize;
	STC u32 smFaceSize;

	void init(u32 vertexCount, u32 facesCount);
	Mesh *addVertex(CNS Vector3 &vec);
	Mesh *addNormal(CNS Vector3 &vec);
	Mesh *addTexCoord(f32 u, f32 v);
	Mesh *addColor(f32 r, f32 g, f32 b, f32 a);
	Mesh *addFace(u32 v1, u32 v2, u32 v3);
	void clear();

	STC Mesh *getRectangle();
	STC void freeRectangle();
};