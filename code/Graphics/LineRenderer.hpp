#pragma once

#include "Core/Core.hpp"

class Shader;
class Vector3;
class Vector4;

class LineRenderer: public ObjectBase
{
    GENERATE_METADATA(LineRenderer)
	PRI_M(Shader *, ShaderLine, NONE)

	PRI_M(u32, MaxShapes, NONE)
	PRI_M(u32, ShapesCounter, NONE)

	PRI_M(u32, VAO, NONE)
	PRI_M(u32, VBOPosition, NONE)
	PRI_M(u32, VBOColor, NONE)
	PRI_M(u32, EBO, NONE)
	PRI_M(SVec(f32), PositionBuffer, NONE)
	PRI_M(SVec(f32), ColorBuffer, NONE)
	PRI_M(SVec(u32), IndicesBuffer, NONE)

	PRI_M(f32, Size, NONE)

	PUB_M(bool, IsWorldSpace, NONE)

PUB
	LineRenderer();
	~LineRenderer() override;

	void init();
	void add(const Vector3 &start, const Vector3 &end, const Vector4 &color);
	void bind();
	void render();
};
