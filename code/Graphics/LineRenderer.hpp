#pragma once

#include "Maths/Vector3.hpp"
#include <vector>

class Shader;

CLASS(LineRenderer, ObjectBase) {
	PRI(ShaderLine, NONE, Shader*)
		
	PRI(MaxShapes, NONE, u32)
	PRI(ShapesCounter, NONE, u32)

	PRI(VAO, NONE, u32)
	PRI(VBOPosition, NONE, u32)
	PRI(EBO, NONE, u32)
	PRI(PositionBufferIndex, NONE, u32)
	PRI(PositionBuffer, NONE, std::vector<f32>)
	PRI(IndicesBuffer, NONE, std::vector<u32>)

	PRI(Size, NONE, f32)

	PUB(IsAffectedByProjection, NONE, bool)
	
public:
	
	LineRenderer();
	virtual ~LineRenderer() override;

	void init();
	void add(const Vector3 &start, const Vector3 &end);
	void bind();
	void render();
};
