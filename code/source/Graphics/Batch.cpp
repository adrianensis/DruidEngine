#include "Batch.h"
#include "Material.h"
#include "List.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderContext.h"
#include "List.h"
#include "Debug.h"
#include "GameObject.h"
#include "Transform.h"
#include "Matrix4.h"

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
	Memory::free<List<Renderer*>>(mRenderers);
	Memory::free<Shader>(mShader);

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

void Batch::init(Mesh* mesh, Material* material) {
	mRenderers = Memory::allocate<List<Renderer*>>();
	mRenderers->init();

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

	u32 i=0;
	for (auto it = mRenderers->getIterator(); !it.isNull(); it.next()){
		//ECHO("RENDERER NUM")
		//VAL(u32,i+1)

		const Matrix4& translationMatrix = it.get()->getGameObject()->getTransform()->getTranslationMatrix();

		mShader->addMatrix(translationMatrix, "translationMatrix");

		glDrawElements(GL_TRIANGLES, mMesh->getFacesData()->getLength(), GL_UNSIGNED_INT, 0);
	}

	RenderContext::enableVAO(0);

}

void Batch::setMesh(Mesh* mesh){
  mMesh = mesh;
}

void Batch::addRenderer(Renderer* renderer) {
	mRenderers->pushBack(renderer);
}

} /* namespace DE */
