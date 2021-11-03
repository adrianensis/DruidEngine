#pragma once

#include "Core/Core.hpp"

class Vector3;


class Mesh: public ObjectBase
{
    GENERATE_METADATA(Mesh)
	PRI std::vector<f32> mVertices = {}; GETREF_CONST(Vertices)
	PRI std::vector<f32> mNormals = {}; GETREF_CONST(Normals)
	PRI std::vector<f32> mTextureCoordinates = {}; GETREF_CONST(TextureCoordinates)
	PRI std::vector<f32> mColors = {}; GETREF_CONST(Colors)
	PRI std::vector<u32> mFaces = {}; GETREF_CONST(Faces)

	PRI u32 mVertexCount = {}; GET(VertexCount)
	PRI u32 mFacesCount = {}; GET(FacesCount)

PRI
	static Mesh *smRectangle;

PUB
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