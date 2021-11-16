#include "Graphics/ShapeBatchRenderer.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Core/EngineConfig.hpp"
#include "Maths/Vector3.hpp"

ShapeBatchRenderer::~ShapeBatchRenderer()
{
	DELETE(mShaderLine);

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);

	mPositionBuffer.clear();
	mColorBuffer.clear();
	mIndicesBuffer.clear();

	mShapesCounter = 0;
}

void ShapeBatchRenderer::init(bool isWorldSpace, u32 verticesPerShape)
{
	mMaxShapes = EngineConfig::getInstance().getConfig().at("line").at("count").get<f32>();

	mIsWorldSpace = isWorldSpace;

	mVerticesPerShape = verticesPerShape;

	mPositionBuffer.reserve(mMaxShapes * 2 * 3); // 2 vertex per line * 3 floats per vertex
	mColorBuffer.reserve(mMaxShapes * 2 * 4); // 2 vertex per line * 4 floats per vertex
	mIndicesBuffer.reserve(mMaxShapes * 2);		 // 1 index per vertex

	mShaderLine = NEW(Shader);
	mShaderLine->initDebug();

	bind();
}

void ShapeBatchRenderer::bind()
{
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(3, 0);
	mVBOColor = RenderContext::createVBO(4, 1);
	mEBO = RenderContext::createEBO();

	RenderContext::resizeVBO(mVBOPosition, mPositionBuffer.capacity());
	RenderContext::resizeVBO(mVBOColor, mColorBuffer.capacity());

	FOR_RANGE(i, 0, mMaxShapes * mVerticesPerShape)
	{
		mIndicesBuffer.push_back(i);
	}

	RenderContext::resizeEBO(mEBO, mIndicesBuffer.size());
	RenderContext::setDataEBO(mEBO, mIndicesBuffer);

	RenderContext::enableVAO(0);
}

void ShapeBatchRenderer::render()
{
	if (mShapesCounter > 0)
	{
		mShaderLine->use();
		RenderContext::enableVAO(mVAO);

		const Matrix4 &projectionMatrix = RenderEngine::getInstance().getCamera()->getProjectionMatrix();
		const Matrix4 &viewMatrix = RenderEngine::getInstance().getCamera()->getViewMatrix();

		mShaderLine->addMatrix(mIsWorldSpace ? projectionMatrix : Matrix4::getIdentity(), "projectionMatrix");
		mShaderLine->addMatrix(mIsWorldSpace ? viewMatrix : Matrix4::getIdentity(), "viewMatrix");

		RenderContext::setDataVBO(mVBOPosition, mPositionBuffer);
		RenderContext::setDataVBO(mVBOColor, mColorBuffer);
		RenderContext::drawLines(mShapesCounter);

		RenderContext::enableVAO(0);

		mPositionBuffer.clear();
		mColorBuffer.clear();
		mShapesCounter = 0;
	}
}

void ShapeBatchRenderer::addPosition(const Vector3& position)
{
	mPositionBuffer.push_back(position.x);
	mPositionBuffer.push_back(position.y);
	mPositionBuffer.push_back(position.z);
}

void ShapeBatchRenderer::addColor(const Vector4& color)
{
	mColorBuffer.push_back(color.x);
	mColorBuffer.push_back(color.y);
	mColorBuffer.push_back(color.z);
	mColorBuffer.push_back(color.w);
}

template<>
void ShapeBatchRenderer::addSpecificShape<Line>(const Line& shape, const Vector4 &color)
{
	addPosition(shape.getStart());
	addPosition(shape.getEnd());

	addColor(color);
	addColor(color);
}

void ShapeBatchRendererMap::render()
{
	FOR_MAP(it, mShapeBatchMap)
	{
		it->second->render();
	}
}

void ShapeBatchRendererMap::terminate()
{
	MAP_DELETE_CONTENT(mShapeBatchMap);
}