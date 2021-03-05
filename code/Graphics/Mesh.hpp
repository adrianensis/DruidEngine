#pragma once

#include "Containers/Array.hpp"
#include "Containers/DynamicArray.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Core/DE_Class.hpp"

namespace DE {

class Mesh: public DE_Class {

private:

	DE_M_GET(Vertices, Array<f32>*)
	DE_M_GET(Normals, Array<f32>*)
	DE_M_GET(TextureCoordinates, Array<f32>*)
	DE_M_GET(Faces, Array<u32>*)

	DE_M(VertexCount, u32)

	DE_M(VerticesIndex, u32)
	DE_M(NormalsIndex, u32)
	DE_M(FacesIndex, u32)
	DE_M(TextureCoordinatesIndex, u32)

	static Mesh* smRectangle;

public:

	DE_CLASS_BODY(Mesh)

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	static Mesh* getRectangle();
	static void freeRectangle();
};

}

