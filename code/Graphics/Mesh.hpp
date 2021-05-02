#pragma once

#include "Containers/Array.hpp"
#include "Containers/DynamicArray.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Core/ObjectBase.hpp"

namespace DE {

class Mesh : public ObjectBase {

private:

	 Array<f32>* mVertices;
	 Array<f32>* mNormals;
	 Array<f32>* mTextureCoordinates;
	 Array<u32>* mFaces;

	 u32 mVertexCount;

	 u32 mVerticesIndex;
	 u32 mNormalsIndex;
	 u32 mFacesIndex;
	 u32 mTextureCoordinatesIndex;

	static Mesh* smRectangle;

public:

	GENERATE_METADATA(Mesh);

	Mesh();
	virtual ~Mesh() override;;

	GET(Vertices);
	GET(Normals);
	GET(TextureCoordinates);
	GET(Faces);

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	static Mesh* getRectangle();
	static void freeRectangle();
};
}

