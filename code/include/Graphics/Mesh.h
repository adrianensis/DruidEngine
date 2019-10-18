#ifndef DE_MESH_H
#define DE_MESH_H

#include "Array.h"
#include "DynamicArray.h"
#include "Vector3.h"
#include "Vector4.h"
#include "DE_Class.h"

namespace DE {

class Mesh : public DE_Class{

private:

	u32 mVertexCount;

	Array<f32>* mVertices;
	Array<f32>* mColors;
	Array<f32>* mNormals;
	Array<f32>* mTextureCoordinates;
	Array<u32>* mFaces;
	DynamicArray<u32>* mFacesTmp;

	u32 mVerticesIndex, mColorsIndex, mNormalsIndex, mFacesIndex, mTextureCoordinatesIndex;

	static Mesh* smRectangle;

public:

	Mesh();
	~Mesh() override;

	void init(const  u32 vertexCount);
	Mesh* addVertex(const Vector3 vec);
	Mesh* addColor(const Vector4 vec);
	Mesh* addNormal(const Vector3 vec);
	Mesh* addTexCoord(u32 u,u32 v);
	Mesh* addFace(u32 v1,u32 v2,u32 v3);
	Mesh* open();
	void close();

	DE_GET(const Array<f32>*, Vertices);
	DE_GET(const Array<f32>*, Colors);
	DE_GET(const Array<f32>*, Normals);
	DE_GET(const Array<f32>*, TextureCoordinates);
	DE_GET(const Array<u32>*, Faces);

	static Mesh* getRectangle();
};

} /* namespace DE */

#endif /* MESH_H */
