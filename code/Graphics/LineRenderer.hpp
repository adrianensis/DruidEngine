#pragma once

#include "Core/Core.hpp"

class Shader;
class Vector3;
class Vector4;

class LineRenderer: public ObjectBase
{
    GENERATE_METADATA(LineRenderer)
	PRI Shader* mShaderLine = nullptr;

	PRI u32 mMaxShapes = 0;
	PRI u32 mShapesCounter = 0;

	PRI u32 mVAO = 0;
	PRI u32 mVBOPosition = 0;
	PRI u32 mVBOColor = 0;
	PRI u32 mEBO = 0;
	PRI std::vector<f32> mPositionBuffer;
	PRI std::vector<f32> mColorBuffer;
	PRI std::vector<u32> mIndicesBuffer;

	PRI f32 mSize = 0.0f;

	PUB bool mIsWorldSpace = false;

PUB
	LineRenderer();
	~LineRenderer() override;

	void init();
	void add(const Vector3 &start, const Vector3 &end, const Vector4 &color);
	void bind();
	void render();
};
