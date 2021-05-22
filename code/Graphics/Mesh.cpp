#include "Graphics/Mesh.hpp"

Mesh* Mesh::smRectangle = nullptr;

void Mesh::init(u32 vertexCount, u32 facesCount) {
	TRACE()

	mVertexCount = vertexCount;

	mVertices.reserve(vertexCount * 3);
	mTextureCoordinates.reserve(vertexCount * 3);
	mNormals.reserve(vertexCount * 3);
	mFaces.reserve(facesCount * 3);

	mVerticesIndex = 0; // Vertices index
	mNormalsIndex = 0; // Normals index
	mFacesIndex = 0; // Face index
	mTextureCoordinatesIndex = 0; // TexCoord index
}

Mesh* Mesh::addVertex(const Vector3 &vector) {
	mVertices[mVerticesIndex] = vector.x;
	mVerticesIndex++;
	mVertices[mVerticesIndex] = vector.y;
	mVerticesIndex++;
	mVertices[mVerticesIndex] = vector.z;
	mVerticesIndex++;
	return this;
}

Mesh* Mesh::addNormal(const Vector3 &vector) {
	mNormals[mNormalsIndex] = vector.x;
	mNormalsIndex++;
	mNormals[mNormalsIndex] = vector.y;
	mNormalsIndex++;
	mNormals[mNormalsIndex] = vector.z;
	mNormalsIndex++;
	return this;
}

Mesh* Mesh::addTexCoord(u32 u, u32 v) {
	mTextureCoordinates[mTextureCoordinatesIndex] = u;
	mTextureCoordinatesIndex++;
	mTextureCoordinates[mTextureCoordinatesIndex] = v;
	mTextureCoordinatesIndex++;
	return this;
}

Mesh* Mesh::addFace(u32 v1, u32 v2, u32 v3) {
	mFaces[mFacesIndex] = v1;
	mFacesIndex++;
	mFaces[mFacesIndex] = v2;
	mFacesIndex++;
	mFaces[mFacesIndex] = v3;
	mFacesIndex++;
	return this;
}

Mesh* Mesh::getRectangle() {
	if (!smRectangle) {
		smRectangle = new Mesh;
		smRectangle->init(4, 2);

		smRectangle->addVertex(Vector3(-0.5f, 0.5f, 0.0f))-> // top left
		addVertex(Vector3(-0.5f, -0.5f, 0.0f))-> // bottom left
		addVertex(Vector3(0.5f, -0.5f, 0.0f))-> // bottom right
		addVertex(Vector3(0.5f, 0.5f, 0.0f))-> // top right

		addTexCoord(0.0f, 1.0f)-> // top left
		addTexCoord(0.0f, 0.0f)-> // bottom left
		addTexCoord(1.0f, 0.0f)-> // bottom right
		addTexCoord(1.0f, 1.0f)-> // top right

		addFace(0, 1, 3)->addFace(1, 2, 3);
	}

	return smRectangle;
}

void Mesh::freeRectangle() {
	delete smRectangle;
	smRectangle = nullptr;
}