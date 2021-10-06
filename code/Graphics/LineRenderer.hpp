#pragma once

#include "Core/Core.hpp"

class Shader;
class Vector3;
class Vector4;

CLASS(LineRenderer, ObjectBase)
{
	PRI(Shader *, ShaderLine, NONE)

	PRI(u32, MaxShapes, NONE)
	PRI(u32, ShapesCounter, NONE)

	PRI(u32, VAO, NONE)
	PRI(u32, VBOPosition, NONE)
	PRI(u32, VBOColor, NONE)
	PRI(u32, EBO, NONE)
	PRI(SVec(f32), PositionBuffer, NONE)
	PRI(SVec(f32), ColorBuffer, NONE)
	PRI(SVec(u32), IndicesBuffer, NONE)

	PRI(f32, Size, NONE)

	PUB(bool, IsWorldSpace, NONE)

public:
	LineRenderer();
	~LineRenderer() OVR;

	void init();
	void add(const Vector3 &start, const Vector3 &end, const Vector4 &color);
	void bind();
	void render();
};
