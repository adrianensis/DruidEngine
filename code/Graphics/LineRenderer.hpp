#pragma once

#include "Core/ObjectBase.hpp"
#include <vector>

class Shader;
class Vector3;
class Vector4;

CLASS(LineRenderer, ObjectBase)
{
	PRI(ShaderLine, NONE, Shader *)

	PRI(MaxShapes, NONE, u32)
	PRI(ShapesCounter, NONE, u32)

	PRI(VAO, NONE, u32)
	PRI(VBOPosition, NONE, u32)
	PRI(VBOColor, NONE, u32)
	PRI(EBO, NONE, u32)
	PRI(PositionBuffer, NONE, std::vector<f32>)
	PRI(ColorBuffer, NONE, std::vector<f32>)
	PRI(IndicesBuffer, NONE, std::vector<u32>)

	PRI(Size, NONE, f32)

	PUB(IsWorldSpace, NONE, bool)

public:
	LineRenderer();
	~LineRenderer() override;

	void init();
	void add(const Vector3 &start, const Vector3 &end, const Vector4 &color);
	void bind();
	void render();
};
