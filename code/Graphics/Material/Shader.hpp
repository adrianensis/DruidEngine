#pragma once

#include "Core/Core.hpp"
#include "Core/BasicTypes.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"

class Matrix4;

class Shader: public ObjectBase
{
    GENERATE_METADATA(Shader)
PRI
	PRI u32 mVertexShader = {};
	PRI u32 mFragmentShader = {};
	PRI u32 mProgram = {};

	static Shader *msShaderDefault;
	static Shader *msShaderDebug;

	void initInternal(const std::string &vertex, const std::string &fragment);

PUB
	Shader();
	~Shader() override;

	static Shader *getDefaultShader();
	static Shader *getDebugShader();

	static void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(const Matrix4 &matrix, const std::string &name);
	void addInt(i32 value, const std::string &name);
	void addUInt(u32 value, const std::string &name);
	void addFloat(f32 value, const std::string &name);
	void addVector4(const Vector4 &value, const std::string &name);
	void addVector3(const Vector3 &value, const std::string &name);
	void addVector2(const Vector2 &value, const std::string &name);
	void addBool(bool value, const std::string &name);
};