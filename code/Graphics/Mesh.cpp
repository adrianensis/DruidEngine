#include "Graphics/Mesh.hpp"
#include <algorithm>

Mesh* Mesh::smRectangle = nullptr;

u32 Mesh::smVertexPositionSize = 3;
u32 Mesh::smVertexNormalSize = 3;
u32 Mesh::smVertexTexCoordSize = 2;
u32 Mesh::smVertexColorSize = 4;
u32 Mesh::smFaceSize = 3;

void Mesh::init(u32 vertexCount, u32 facesCount) {
	TRACE()

	mVertexCount = vertexCount;
	mFacesCount = facesCount;

	clear();
}

Mesh* Mesh::addVertex(const Vector3 &vector) {
	mVertices.push_back(vector.x);
	mVertices.push_back(vector.y);
	mVertices.push_back(vector.z);
	return this;
}

Mesh* Mesh::addNormal(const Vector3 &vector) {
	mNormals.push_back(vector.x);
	mNormals.push_back(vector.y);
	mNormals.push_back(vector.z);
	return this;
}

Mesh* Mesh::addTexCoord(u32 u, u32 v) {
	mTextureCoordinates.push_back(u);
	mTextureCoordinates.push_back(v);
	return this;
}

Mesh* Mesh::addColor(f32 r, f32 g, f32 b, f32 a) {
	mColors.push_back(r);
	mColors.push_back(g);
	mColors.push_back(b);
	mColors.push_back(a);
	return this;
}

Mesh* Mesh::addFace(u32 v1, u32 v2, u32 v3) {
	mFaces.push_back(v1);
	mFaces.push_back(v2);
	mFaces.push_back(v3);
	return this;
}

void Mesh::clear() {
	mVertices.clear();
	mNormals.clear();
	mTextureCoordinates.clear();
	mColors.clear();
	mFaces.clear();

	mVertices.reserve(mVertexCount * smVertexPositionSize);
	mTextureCoordinates.reserve(mVertexCount * smVertexTexCoordSize);
	mColors.reserve(mVertexCount * smVertexColorSize);
	mNormals.reserve(mVertexCount * smVertexNormalSize);
	mFaces.reserve(mFacesCount * smFaceSize);

	std::fill(mColors.begin(), mColors.end(), 0);
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