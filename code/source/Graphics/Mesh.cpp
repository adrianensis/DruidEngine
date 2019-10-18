#include "Mesh.h"
#include "Memory.h"

namespace DE {

// ---------------------------------------------------------------------------

Mesh* Mesh::smRectangle = nullptr;

Mesh::Mesh() : DE_Class() {

	mVertexCount = 0;
	mVertices = nullptr;
	mColors = nullptr;
	mNormals = nullptr;
	mFaces = nullptr;
	mFacesTmp = nullptr;
	mTextureCoordinates = nullptr;
	mVerticesIndex = 0;
	mColorsIndex = 0;
	mNormalsIndex = 0;
	mFacesIndex = 0;
	mTextureCoordinatesIndex = 0;
}

Mesh::~Mesh() {
	Memory::free<Array<f32>>(mVertices);
	Memory::free<Array<f32>>(mColors);
	Memory::free<Array<f32>>(mNormals);
	Memory::free<Array<f32>>(mTextureCoordinates);
	Memory::free<Array<u32>>(mFaces);

	if(mFacesTmp != nullptr)
	{
		Memory::free<DynamicArray<u32>>(mFacesTmp);
	}
}

// ---------------------------------------------------------------------------

void Mesh::init(u32 vertexCount) {
	mVertexCount = vertexCount;

	mVertices = Memory::allocate<Array<f32>>();
	mTextureCoordinates = Memory::allocate<Array<f32>>();
	mColors = Memory::allocate<Array<f32>>();
	mNormals = Memory::allocate<Array<f32>>();
	mFaces = Memory::allocate<Array<u32>>();

	mVertices->init(vertexCount*3);
	mTextureCoordinates->init(vertexCount*3);
	mColors->init(vertexCount*4);
	mNormals->init(vertexCount*3);

	mVerticesIndex = 0; // Vertices index
	mNormalsIndex = 0; // Normals index
	mFacesIndex = 0; // Face index
	mTextureCoordinatesIndex = 0; // TexCoord index
}

// ---------------------------------------------------------------------------

Mesh* Mesh::addVertex(const Vector3 vector) {
	mVertices->set(mVerticesIndex,vector.x); mVerticesIndex++;
	mVertices->set(mVerticesIndex,vector.y); mVerticesIndex++;
	mVertices->set(mVerticesIndex,vector.z); mVerticesIndex++;
	return this;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::addColor(const Vector4 vector) {
	mColors->set(mColorsIndex,vector.x); mColorsIndex++;
	mColors->set(mColorsIndex,vector.y); mColorsIndex++;
	mColors->set(mColorsIndex,vector.z); mColorsIndex++;
	mColors->set(mColorsIndex,vector.w); mColorsIndex++;
	return this;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::addNormal(const Vector3 vector) {
	mNormals->set(mNormalsIndex,vector.x); mNormalsIndex++;
	mNormals->set(mNormalsIndex,vector.y); mNormalsIndex++;
	mNormals->set(mNormalsIndex,vector.z); mNormalsIndex++;
    return this;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::addTexCoord(u32 u,u32 v) {
  mTextureCoordinates->set(mTextureCoordinatesIndex,u); mTextureCoordinatesIndex++;
  mTextureCoordinates->set(mTextureCoordinatesIndex,v); mTextureCoordinatesIndex++;
  return this;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::addFace(u32 v1,u32 v2,u32 v3) {
  mFacesTmp->set(mFacesIndex,v1); mFacesIndex++;
  mFacesTmp->set(mFacesIndex,v2); mFacesIndex++;
  mFacesTmp->set(mFacesIndex,v3); mFacesIndex++;
  return this;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::open() {
	mFacesTmp = Memory::allocate<DynamicArray<u32>>();
	mFacesTmp->init();
	return this;
}

// ---------------------------------------------------------------------------

void Mesh::close() {
	// move data from dynamic array to fixed array.
	mFaces->init(mFacesTmp->getLength());
	mFaces->put(*mFacesTmp,0,0);

	Memory::free<DynamicArray<u32>>(mFacesTmp);
	mFacesTmp = nullptr;
}

// ---------------------------------------------------------------------------

Mesh* Mesh::getRectangle() {
	if( ! smRectangle) {
		smRectangle = Memory::allocate<Mesh>();
		smRectangle->init(4);

		smRectangle->open()->
			addVertex(Vector3(-0.5f, 0.5f, 0.0f))-> // top left
			addVertex(Vector3(-0.5f, -0.5f, 0.0f))-> // bottom left
			addVertex(Vector3(0.5f, -0.5f, 0.0f))-> // bottom right
			addVertex(Vector3(0.5f, 0.5f, 0.0f))-> // top right

			addTexCoord(0.0f, 1.0f)-> // top left
			addTexCoord(0.0f, 0.0f)-> // bottom left
			addTexCoord(1.0f, 0.0f)-> // bottom right
			addTexCoord(1.0f, 1.0f)-> // top right

			addColor(Vector4(0, 1, 1, 1))-> // top left
			addColor(Vector4(1, 0, 0, 1))-> // bottom left
			addColor(Vector4(1, 1, 0, 1))-> // bottom right
			addColor(Vector4(0, 0, 1, 1))-> // top right

			addFace(0,1,3)->
			addFace(1,2,3)->
			close();
	}

	return smRectangle;
}


// ---------------------------------------------------------------------------

} /* namespace DE */
