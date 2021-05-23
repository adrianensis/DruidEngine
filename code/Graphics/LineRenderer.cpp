#include "Graphics/LineRenderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera.hpp"

LineRenderer::LineRenderer() {
	mIsAffectedByProjection = true;
}

LineRenderer::~LineRenderer() {
	delete mShaderLine;

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

void LineRenderer::init() {
	//TRACE();

	mMaxShapes = 350; //EngineConfig::getInstance()->getF32("line.renderers.count");

	mPositionBuffer.reserve(mMaxShapes * 2 * 3); // 2 vertex per line * 3 floats per vertex
	mIndicesBuffer.reserve(mMaxShapes * 2); // 1 index per vertex

	mShaderLine = new Shader;
	mShaderLine->initDebug();

	bind();
}

void LineRenderer::add(const Vector3 &start, const Vector3 &end) {
	if(mShapesCounter < mMaxShapes){
		mPositionBuffer[mPositionBufferIndex] = start.x; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = start.y; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = start.z; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = end.x; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = end.y; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = end.z; mPositionBufferIndex++;

		mShapesCounter++;
	}
}

void LineRenderer::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(3, 0);
	mEBO = RenderContext::createEBO();

	FOR_RANGE(i, 0, mMaxShapes*2) {
		mIndicesBuffer[i] = i;	
	}

	RenderContext::setDataEBO(mEBO, mIndicesBuffer);

	RenderContext::enableVAO(0);
}

void LineRenderer::render() {
	if(mShapesCounter > 0) {
		mShaderLine->use();
		RenderContext::enableVAO(mVAO);

		const Matrix4& projectionMatrix = RenderEngine::getInstance()->getCamera()->getProjectionMatrix();
		const Matrix4& viewTranslationMatrix = RenderEngine::getInstance()->getCamera()->getViewTranslationMatrix();
		const Matrix4& viewRotationMatrix = RenderEngine::getInstance()->getCamera()->getViewRotationMatrix();

		if(mIsAffectedByProjection) {
			mShaderLine->addMatrix(projectionMatrix, "projectionMatrix");
			mShaderLine->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
			mShaderLine->addMatrix(viewRotationMatrix, "viewRotationMatrix");
		} else {
			mShaderLine->addMatrix(Matrix4::getIdentity(), "projectionMatrix");
			mShaderLine->addMatrix(Matrix4::getIdentity(), "viewTranslationMatrix");
			mShaderLine->addMatrix(Matrix4::getIdentity(), "viewRotationMatrix");
		}
		
		RenderContext::setDataVBO(mVBOPosition, mPositionBuffer);
		RenderContext::drawLines(mShapesCounter);

		RenderContext::enableVAO(0);

		mPositionBufferIndex = 0;
		mShapesCounter = 0;
	}
}