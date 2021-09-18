#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"

class Matrix4;

CLASS(Shader, ObjectBase)
{
private:
	PRI(VertexShader, NONE, u32)
	PRI(FragmentShader, NONE, u32)
	PRI(Program, NONE, u32)

	STC Shader *msShaderDefault;
	STC Shader *msShaderDebug;

	void initInternal(CNS SStr &vertex, CNS SStr &fragment);

public:
	Shader();
	~Shader() OVR;

	STC Shader *getDefaultShader();
	STC Shader *getDebugShader();

	STC void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(CNS Matrix4 &matrix, CNS SStr &name);
	void addInt(i32 value, CNS SStr &name);
	void addUInt(u32 value, CNS SStr &name);
	void addFloat(f32 value, CNS SStr &name);
	void addVector4(CNS Vector4 &value, CNS SStr &name);
	void addVector3(CNS Vector3 &value, CNS SStr &name);
	void addVector2(CNS Vector2 &value, CNS SStr &name);
	void addBool(bool value, CNS SStr &name);
};