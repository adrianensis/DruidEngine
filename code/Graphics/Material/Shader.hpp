#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"

class Matrix4;

CLASS(Shader, ObjectBase)
{
PRI
	PRI_M(u32, VertexShader, NONE)
	PRI_M(u32, FragmentShader, NONE)
	PRI_M(u32, Program, NONE)

	static Shader *msShaderDefault;
	static Shader *msShaderDebug;

	void initInternal(const SStr &vertex, const SStr &fragment);

PUB
	Shader();
	~Shader() override;

	static Shader *getDefaultShader();
	static Shader *getDebugShader();

	static void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(const Matrix4 &matrix, const SStr &name);
	void addInt(i32 value, const SStr &name);
	void addUInt(u32 value, const SStr &name);
	void addFloat(f32 value, const SStr &name);
	void addVector4(const Vector4 &value, const SStr &name);
	void addVector3(const Vector3 &value, const SStr &name);
	void addVector2(const Vector2 &value, const SStr &name);
	void addBool(bool value, const SStr &name);
};