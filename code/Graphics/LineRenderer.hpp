#pragma once

#include "Core/Core.hpp"

class Shader;
class Vector3;
class Vector4;

class LineRenderer: public ObjectBase
{
    GENERATE_METADATA(LineRenderer)
	PRI Shader * mShaderLine = {};

	PRI u32 mMaxShapes = {};
	PRI u32 mShapesCounter = {};

	PRI u32 mVAO = {};
	PRI u32 mVBOPosition = {};
	PRI u32 mVBOColor = {};
	PRI u32 mEBO = {};
	PRI std::vector<f32> mPositionBuffer = {};
	PRI std::vector<f32> mColorBuffer = {};
	PRI std::vector<u32> mIndicesBuffer = {};

	PRI f32 mSize = {};

	PUB bool mIsWorldSpace = {};

PUB
	LineRenderer();
	~LineRenderer() override;

	void init();
	void add(const Vector3 &start, const Vector3 &end, const Vector4 &color);
	void bind();
	void render();
};
