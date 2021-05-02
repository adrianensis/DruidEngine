#pragma once

#include "Maths/Vector3.hpp"

namespace DE {

class Shader;
template<class T> class Array;

class LineRenderer : public ObjectBase {
private:
		 Shader* mShaderLine;
		
		 u32 mMaxShapes;
		 u32 mShapesCounter;

		 u32 mVAO;
		 u32 mVBOPosition;
		 u32 mEBO;
		 u32 mPositionBufferIndex;
		 Array<f32>* mPositionBuffer;
		 Array<u32>* mIndicesBuffer;

		 f32 mSize;
public:
		GENERATE_METADATA(LineRenderer);

	LineRenderer();
	virtual ~LineRenderer() override;
		 bool mIsAffectedByProjection;
		
		void init();
		void add(const Vector3 &start, const Vector3 &end);
		void bind();
		void render();
	};
	
}
