#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Maths/Vector4.hpp"

#include <vector>

class Matrix4;

CLASS(Shader, ObjectBase) {
private:
	PRI(VertexShader, NONE, u32)
	PRI(FragmentShader, NONE, u32)
	PRI(Program, NONE, u32)

	static Shader* msShaderDefault;
	static Shader* msShaderDebug;

	void initInternal(const std::string& vertex, const std::string& fragment);

public:

	Shader();
	~Shader() override;
	
	static Shader* getDefaultShader();
	static Shader* getDebugShader();

	static void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(const Matrix4 &matrix, const std::string& name);
	void addInt(i32 value, const std::string& name);
	void addUInt(u32 value, const std::string& name);
	void addFloat(f32 value, const std::string& name);
	void addVector4(std::vector<f32>* value, const std::string& name);
	void addVector3(std::vector<f32>* value, const std::string& name);
	void addBool(bool value, const std::string& name);
};