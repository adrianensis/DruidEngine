#include "Graphics/LineRenderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Core/EngineConfig.hpp"
#include "Maths/Vector3.hpp"

LineRenderer::LineRenderer()
{
	mIsWorldSpace = true;
}

LineRenderer::~LineRenderer()
{
	DELETE(mShaderLine);

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

void LineRenderer::init()
{
	mMaxShapes = EngineConfig::getInstance()->getConfig().at("line").at("count").get<f32>();

	mPositionBuffer.reserve(mMaxShapes * 2 * 3); // 2 vertex per line * 3 floats per vertex
	mColorBuffer.reserve(mMaxShapes * 2 * 4); // 2 vertex per line * 4 floats per vertex
	mIndicesBuffer.reserve(mMaxShapes * 2);		 // 1 index per vertex

	mShaderLine = NEW(Shader);
	mShaderLine->initDebug();

	bind();
}

void LineRenderer::add(CNS Vector3 &start, CNS Vector3 &end, CNS Vector4 &color)
{
	if (mShapesCounter < mMaxShapes)
	{
		mPositionBuffer.push_back(start.x);
		mPositionBuffer.push_back(start.y);
		mPositionBuffer.push_back(start.z);
		mPositionBuffer.push_back(end.x);
		mPositionBuffer.push_back(end.y);
		mPositionBuffer.push_back(end.z);

		mColorBuffer.push_back(color.x);
		mColorBuffer.push_back(color.y);
		mColorBuffer.push_back(color.z);
		mColorBuffer.push_back(color.w);
		mColorBuffer.push_back(color.x);
		mColorBuffer.push_back(color.y);
		mColorBuffer.push_back(color.z);
		mColorBuffer.push_back(color.w);

		mShapesCounter++;
	}
}

void LineRenderer::bind()
{
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(3, 0);
	mVBOColor = RenderContext::createVBO(4, 1);
	mEBO = RenderContext::createEBO();

	FOR_RANGE(i, 0, mMaxShapes * 2)
	{
		mIndicesBuffer.push_back(i);
	}

	RenderContext::setDataEBO(mEBO, mIndicesBuffer);

	RenderContext::enableVAO(0);
}

void LineRenderer::render()
{
	if (mShapesCounter > 0)
	{
		mShaderLine->use();
		RenderContext::enableVAO(mVAO);

		CNS Matrix4 &projectionMatrix = RenderEngine::getInstance()->getCamera()->getProjectionMatrix();
		CNS Matrix4 &viewTranslationMatrix = RenderEngine::getInstance()->getCamera()->getViewTranslationMatrix();
		CNS Matrix4 &viewRotationMatrix = RenderEngine::getInstance()->getCamera()->getViewRotationMatrix();

		if (mIsWorldSpace)
		{
			mShaderLine->addMatrix(projectionMatrix, "projectionMatrix");
			mShaderLine->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
			mShaderLine->addMatrix(viewRotationMatrix, "viewRotationMatrix");
		}
		else
		{
			mShaderLine->addMatrix(Matrix4::getIdentity(), "projectionMatrix");
			mShaderLine->addMatrix(Matrix4::getIdentity(), "viewTranslationMatrix");
			mShaderLine->addMatrix(Matrix4::getIdentity(), "viewRotationMatrix");
		}

		RenderContext::setDataVBO(mVBOPosition, mPositionBuffer);
		RenderContext::setDataVBO(mVBOColor, mColorBuffer);
		RenderContext::drawLines(mShapesCounter);

		RenderContext::enableVAO(0);

		mPositionBuffer.clear();
		mColorBuffer.clear();
		mShapesCounter = 0;
	}
}