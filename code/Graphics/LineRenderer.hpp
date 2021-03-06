#pragma once

#include "Maths/Vector3.hpp"

namespace DE {

class Shader;
template<class T> class Array;

class LineRenderer: public DE_Class {
private:
		DE_M(ShaderLine, Shader*)
		
		DE_M(MaxShapes, u32)
		DE_M(ShapesCounter, u32)

		DE_M(VAO, u32)
		DE_M(VBOPosition, u32)
		DE_M(EBO, u32)
		DE_M(PositionBufferIndex, u32)
		DE_M(PositionBuffer, Array<f32>*)
		DE_M(IndicesBuffer, Array<u32>*)

		DE_M(Size, f32)
public:
		DE_CLASS_BODY(LineRenderer)
		DE_M(IsAffectedByProjection, bool)
		
		void init();
		void add(const Vector3 &start, const Vector3 &end);
		void bind();
		void render();
	};
	
}
