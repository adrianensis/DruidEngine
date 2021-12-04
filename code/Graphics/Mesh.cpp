#include "Graphics/Mesh.hpp"
#include "Core/Memory.hpp"

#include <algorithm>

Mesh *Mesh::smRectangle = nullptr;

void Mesh::init(u32 vertexCount, u32 facesCount)
{
	mVertexCount = vertexCount;
	mFacesCount = facesCount;

	clear();
}

Mesh *Mesh::addVertex(const Vector3 &vector)
{
	mVertices.push_back(vector.x);
	mVertices.push_back(vector.y);
	mVertices.push_back(vector.z);
	return this;
}

Mesh *Mesh::addNormal(const Vector3 &vector)
{
	mNormals.push_back(vector.x);
	mNormals.push_back(vector.y);
	mNormals.push_back(vector.z);
	return this;
}

Mesh *Mesh::addTexCoord(f32 u, f32 v)
{
	mTextureCoordinates.push_back(u);
	mTextureCoordinates.push_back(v);
	return this;
}

Mesh *Mesh::addColor(f32 r, f32 g, f32 b, f32 a)
{
	mColors.push_back(r);
	mColors.push_back(g);
	mColors.push_back(b);
	mColors.push_back(a);
	return this;
}

Mesh *Mesh::addFace(u16 v1, u16 v2, u16 v3)
{
	addFaceIndex(v1);
	addFaceIndex(v2);
	addFaceIndex(v3);
	return this;
}

Mesh *Mesh::addFaceIndex(u16 i)
{
	mFaces.push_back(i);
	return this;
}

Mesh *Mesh::copyVertices(const Mesh* other)
{
	std::copy(other->getVertices().begin(), other->getVertices().end(), back_inserter(mVertices));
	return this;
}

Mesh *Mesh::copyNormals(const Mesh* other)
{
	std::copy(other->getNormals().begin(), other->getNormals().end(), back_inserter(mNormals));
	return this;
}

Mesh *Mesh::copyTextureCoordinates(const Mesh* other)
{
	std::copy(other->getTextureCoordinates().begin(), other->getTextureCoordinates().end(), back_inserter(mTextureCoordinates));
	return this;
}

Mesh *Mesh::copyColors(const Mesh* other)
{
	std::copy(other->getColors().begin(), other->getColors().end(), back_inserter(mColors));
	return this;
}

Mesh *Mesh::copyFaces(const Mesh* other)
{
	std::copy(other->getFaces().begin(), other->getFaces().end(), back_inserter(mFaces));
	return this;
}

void Mesh::clear()
{
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