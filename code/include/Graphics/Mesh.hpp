#ifndef DE_MESH_H
#define DE_MESH_H

#include "Array.hpp"
#include "DynamicArray.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "DE_Class.hpp"

namespace DE {

class Mesh: public DE_Class {

private:

	u32 mVertexCount;

	Array<f32>* mVertices;
	Array<f32>* mNormals;
	Array<f32>* mTextureCoordinates;
	Array<u32>* mFaces;

	u32 mVerticesIndex, mNormalsIndex, mFacesIndex, mTextureCoordinatesIndex;

	static Mesh* smRectangle;

public:

	DE_CLASS(Mesh, DE_Class)
	;

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	const Array<f32>* getVertices() const {
		return mVertices;
	};

	const Array<f32>* getNormals() const {
		return mNormals;
	};

	const Array<f32>* getTextureCoordinates() const {
		return mTextureCoordinates;
	};

	const Array<u32>* getFaces() const {
		return mFaces;
	};


	static Mesh* getRectangle();
};

} /* namespace DE */

#endif /* MESH_H */
