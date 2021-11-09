#pragma once

#include "Core/Core.hpp"
#include "Maths/Geometry.hpp"

class Shader;
class Vector3;
class Vector4;

class ShapeBatchRenderer: public ObjectBase
{
	GENERATE_METADATA(ShapeBatchRenderer)
	PRI Shader* mShaderLine = nullptr;

	PRO u32 mMaxShapes = 0;
	PRO u32 mShapesCounter = 0;
	PRO u32 mVerticesPerShape = 0;

	PRI u32 mVAO = 0;
	PRI u32 mVBOPosition = 0;
	PRI u32 mVBOColor = 0;
	PRI u32 mEBO = 0;
	PRI std::vector<f32> mPositionBuffer;
	PRI std::vector<f32> mColorBuffer;
	PRI std::vector<u32> mIndicesBuffer;

	PRI f32 mSize = 0.0f;

	PRI bool mIsWorldSpace = true;

PRI
	void bind();

	void addPosition(const Vector3& position);
	void addColor(const Vector4& color);

	template<class T, typename = std::enable_if_t<std::is_base_of<Shape, T>::value> >
	void addSpecificShape(const T& shape, const Vector4 &color)
	{

	}

PUB
	ShapeBatchRenderer() { }
	~ShapeBatchRenderer() override;

	void init(bool isWorldSpace, u32 verticesPerShape);
	void render();
	template<class T, typename = std::enable_if_t<std::is_base_of<Shape, T>::value> >
	void add(const T& shape, const Vector4 &color)
	{
		if(mShapesCounter < mMaxShapes)
		{
			addSpecificShape<T>(shape, color);
			mShapesCounter++;
		}
	}
};

// Shape Batch Renderer Specialization
template<>
void ShapeBatchRenderer::addSpecificShape<Line>(const Line& shape, const Vector4 &color);

// To handle different batches for different shapes
class ShapeBatchRendererMap: public ObjectBase
{
    GENERATE_METADATA(ShapeBatchRendererMap)
	
	PRI std::map<ClassId, ShapeBatchRenderer*> mShapeBatchMap;
	PRI bool mIsWorldSpace = true; GET_SET(IsWorldSpace)

PUB
	void render();
	void terminate();

	template<class T, typename = std::enable_if_t<std::is_base_of<Shape, T>::value> >
	void add(const T& shape, bool isWorldSpace = true, const Vector4& color = Vector4(1, 1, 1, 1))
	{
		ClassId shapeClassId = T::getClassIdStatic();
		if(!MAP_CONTAINS(mShapeBatchMap, shapeClassId))
		{
			ShapeBatchRenderer* shapeBatchRenderer = NEW(ShapeBatchRenderer);
			shapeBatchRenderer->init(mIsWorldSpace, shape.getVerticesCount());
			MAP_INSERT(mShapeBatchMap, shapeClassId, shapeBatchRenderer)
		}

		mShapeBatchMap.at(shapeClassId)->add<T>(shape, color);
	}
};
