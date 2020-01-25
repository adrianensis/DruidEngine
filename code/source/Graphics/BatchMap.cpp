#include "BatchMap.h"
#include "Material.h"
#include "Texture.h"
#include "List.h"
#include "Renderer.h"
#include "RenderEngine.h"
#include "Camera.h"
#include "Frustum.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderContext.h"
#include "List.h"
#include "Log.h"
#include "GameObject.h"
#include "Transform.h"
#include "Matrix4.h"
#include "MathUtils.h"
#include "Time.h"

// ***************************************************
// *								TODO
// *
// * This Batch is not used.
// * It's supposed to be used for Tiled Maps.
// *
// *
// ***************************************************

namespace DE {

// ---------------------------------------------------------------------------

BatchMap::BatchMap() : DE_Class() {
	mVBOPosition = 0;
	mEBO = 0;
	mVBOTexture = 0;
	//mVBOColor = 0;
	mVBONormal = 0;
	mVAO = 0;
	mMesh = nullptr;
	mMaterial = nullptr;
	mRenderEngine = nullptr;
	mTextureId = 0;

	mTilesCount = 0;

	mMapSize = Vector2(0,0);
}

BatchMap::~BatchMap() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

void BatchMap::init(RenderEngine* renderEngine, Material* material, const Vector2& mapSize) {
	TRACE();

	mRenderEngine = renderEngine;
	mMaterial = material;

	mMesh = Memory::allocate<Mesh>();

	u32 totalTiles = mapSize.x * mapSize.y;

	mMesh->init(totalTiles*4, totalTiles*2); // each tile has: 4 vertices and 2 faces (triangles)

	mMapSize = Vector2(mapSize);
}

// ---------------------------------------------------------------------------

void BatchMap::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(mMesh->getVertices(), 3, 0);
	mVBOTexture = RenderContext::createVBO(mMesh->getTextureCoordinates(), 2, 1);
	//mVBOColor = RenderContext::createVBO(mMesh->getColors(), 4, 2);
//	mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO(mMesh->getFaces());

	glGenTextures(1, &mTextureId);

	glBindTexture(GL_TEXTURE_2D, mTextureId);

	Texture* texture = mMaterial->getTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Prevents s-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// Prevents t-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	RenderContext::enableVAO(0);
}

// ---------------------------------------------------------------------------

void BatchMap::update() {

}

// ---------------------------------------------------------------------------

u32 BatchMap::render() {

	Shader* shader = mMaterial->getShader();

	shader->use();
	RenderContext::enableVAO(mVAO);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	Camera* camera = mRenderEngine->getCamera();

	const Matrix4& projectionMatrix = camera->getProjectionMatrix();
	const Matrix4& viewTranslationMatrix = camera->getViewTranslationMatrix();
	const Matrix4& viewRotationMatrix = camera->getViewRotationMatrix();

	shader->addMatrix(projectionMatrix, "projectionMatrix");
	shader->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
	shader->addMatrix(viewRotationMatrix, "viewRotationMatrix");

	u32 drawCallCounter = 0;

	u32 lastLayer = 0;

	shader->addFloat(Time::getDeltaTimeSeconds(), "time");

	glPolygonMode(GL_FRONT_AND_BACK, /*lineMode ? GL_LINE : */GL_FILL);

	glDrawElements(GL_TRIANGLES, mMesh->getFaces()->getLength(), GL_UNSIGNED_INT, 0);

	RenderContext::enableVAO(0);

	return 1; // just 1 drawcall
}

// ---------------------------------------------------------------------------

void BatchMap::addRenderer(Renderer* renderer) {

	u32 faceOffset = 4*mTilesCount;

	Transform* t = renderer->getGameObject()->getTransform();

	const Matrix4& translationMatrix = t->getTranslationMatrix();
	const Matrix4& rotationMatrix = t->getRotationMatrix();
	const Matrix4& scaleMatrix = t->getScaleMatrix();

	Matrix4 model(translationMatrix);

	model.mul(scaleMatrix);

	mMesh->
		addVertex(Vector3(model.mulVector(Vector4(-0.5f, 0.5f, 0.0f, 1.0))))-> // top left
		addVertex(Vector3(model.mulVector(Vector4(-0.5f, -0.5f, 0.0f, 1.0))))-> // bottom left
		addVertex(Vector3(model.mulVector(Vector4(0.5f, -0.5f, 0.0f, 1.0))))-> // bottom right
		addVertex(Vector3(model.mulVector(Vector4(0.5f, 0.5f, 0.0f, 1.0))))-> // top right

		// TODO : calculate correct UV values
		addTexCoord(0.0f, 1.0f)-> // top left
		addTexCoord(0.0f, 0.0f)-> // bottom left
		addTexCoord(1.0f, 0.0f)-> // bottom right
		addTexCoord(1.0f, 1.0f)-> // top right

		addFace(0 + faceOffset, 1 + faceOffset, 3 + faceOffset)->
		addFace(1 + faceOffset, 2 + faceOffset, 3 + faceOffset);

	mTilesCount++;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
