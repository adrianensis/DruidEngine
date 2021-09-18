#pragma once

#include "Core/Core.hpp"

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
	PRI(PositionBuffer, NONE, SVec<f32>)
	PRI(ColorBuffer, NONE, SVec<f32>)
	PRI(IndicesBuffer, NONE, SVec<u32>)

	PRI(Size, NONE, f32)

	PUB(IsWorldSpace, NONE, bool)

public:
	LineRenderer();
	~LineRenderer() OVR;

	void init();
	void add(CNS Vector3 &start, CNS Vector3 &end, CNS Vector4 &color);
	void bind();
	void render();
};
