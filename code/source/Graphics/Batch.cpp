#include "Batch.h"

#include "RenderContext.h"

namespace DE {

Batch::Batch() : DE_Class(),
		mVBOPosition(0),
		mEBO(0),
		mVBOTexture(0),
		mVBOColor(0),
		mVBONormal(0),
		mVAO(0),
		mMesh(nullptr),
		mRenderers(nullptr),
		mMaterial(nullptr),
		mShader(nullptr)
{
}

Batch::~Batch() {

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);

}

void Batch::init(Mesh* mesh, Material* material) {
	mMesh = mesh;
	mMaterial = material;

	mShader = Memory::allocate<Shader>();
	mShader->init();
}

void Batch::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(mMesh->getVerticesData(), 3, 0);
//	mVBOTexture = RenderContext::createVBO(mMesh->getTexureCoordinatesData(), 2, 1);
//	mVBOColor = RenderContext::createVBO(mMesh->getColorsData(), 4, 2);
//	mVBONormal = RenderContext::createVBO(mMesh->getNormalsData(), 3, 3);
	mEBO = RenderContext::createEBO(mMesh->getFacesData());
	RenderContext::enableVAO(0);
}

void Batch::update() {

}

void Batch::render() {

	mShader->use();
	RenderContext::enableVAO(mVAO);
	glDrawElements(GL_TRIANGLES, mMesh->getFacesData()->getLength(), GL_UNSIGNED_INT, 0);
	RenderContext::enableVAO(0);
}

} /* namespace DE */


