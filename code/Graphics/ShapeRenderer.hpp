#pragma once

#include "Core/Core.hpp"
#include "Maths/Geometry.hpp"

class Shader;
class Vector3;
class Vector4;

class ShapeRendererBase: public ObjectBase
{
	GENERATE_METADATA(ShapeRendererBase)
	PRI Shader* mShaderLine = nullptr;

	PRO u32 mMaxShapes = 0;
	PRO u32 mShapesCounter = 0;

	PRI u32 mVAO = 0;
	PRI u32 mVBOPosition = 0;
	PRI u32 mVBOColor = 0;
	PRI u32 mEBO = 0;
	PRI std::vector<f32> mPositionBuffer;
	PRI std::vector<f32> mColorBuffer;
	PRI std::vector<u32> mIndicesBuffer;

	PRI f32 mSize = 0.0f;

	PUB bool mIsWorldSpace = true;

PRO
	void addPosition(const Vector3& position);
	void addColor(const Vector4& color);

PUB
	ShapeRendererBase() { }
	~ShapeRendererBase() override;

	void init();
	void bind();
	void render();
};

template<class T, typename = std::enable_if_t<std::is_base_of<Shape, T>::value> >
class ShapeRenderer: public ShapeRendererBase
{
    GENERATE_METADATA(ShapeRenderer<T>)

PRO
	virtual void addCustom(const T& shape, const Vector4 &color) { }
PUB
	ShapeRenderer() { }

	void add(const T& shape, const Vector4 &color)
	{
		if(mShapesCounter < mMaxShapes)
		{
			addCustom(shape, color);
			mShapesCounter++;
		}
	}
};

class LineRenderer: public ShapeRenderer<Line>
{
    GENERATE_METADATA(LineRenderer)
PRO
	void addCustom(const Line &line, const Vector4 &color) override;
};
