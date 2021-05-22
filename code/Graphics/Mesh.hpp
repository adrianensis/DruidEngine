#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Core/ObjectBase.hpp"

#include <vector>

CLASS(Mesh, ObjectBase) {

	PRI(Vertices, GETREF_CONST, std::vector<f32>)
	PRI(Normals, GETREF_CONST, std::vector<f32>)
	PRI(TextureCoordinates, GETREF_CONST, std::vector<f32>)
	PRI(Faces, GETREF_CONST, std::vector<u32>)

	PRI(VertexCount, NONE, u32)

	PRI(VerticesIndex, NONE, u32)
	PRI(NormalsIndex, NONE, u32)
	PRI(FacesIndex, NONE, u32)
	PRI(TextureCoordinatesIndex, NONE, u32)

private:
	static Mesh* smRectangle;

public:

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	static Mesh* getRectangle();
	static void freeRectangle();
};