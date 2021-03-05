#include "Graphics/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Maths/Matrix4.hpp"
#include "Memory/Memory.hpp"
#include "Containers/Array.hpp"
#include <fstream> // std::ifstream
#include <string>
#include <iostream>

namespace DE {

Shader* Shader::msShaderDefault = nullptr;
Shader* Shader::msShaderDebug = nullptr;

Shader::Shader() : DE_Class() {
	mVertexShader = -1;
	mFragmentShader = -1;
	mProgram = -1;
};

Shader::~Shader() = default;

Shader* Shader::getDefaultShader() {
	if (!msShaderDefault) {
		msShaderDefault = DE_NEW<Shader>();
		msShaderDefault->init();
	}
	return msShaderDefault;
}

Shader* Shader::getDebugShader() {
	if (!msShaderDebug) {
		msShaderDebug = DE_NEW<Shader>();
		msShaderDebug->initDebug();
	}
	return msShaderDebug;
}

void Shader::freeStaticShaders() {
	if (msShaderDefault) {
		DE_FREE(msShaderDefault);
	}

	if (msShaderDebug) {
		DE_FREE(msShaderDebug);
	}
}

void Shader::initInternal(const String &vertex, const String &fragment) {
	DE_TRACE()

	mVertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::ifstream vertex_ifs;
	vertex_ifs.open(vertex.c_str(), std::ifstream::in);
	String vertexShaderSource((std::istreambuf_iterator<char>(vertex_ifs)), (std::istreambuf_iterator<char>()));

	const char* c_str_vertex = vertexShaderSource.c_str();

	glShaderSource(mVertexShader, 1, &c_str_vertex, nullptr);
	glCompileShader(mVertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl << vertexShaderSource
				<< std::endl;
	}

	mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream fragment_ifs;
	fragment_ifs.open(fragment.c_str(), std::ifstream::in);
	String fragmentShaderSource((std::istreambuf_iterator<char>(fragment_ifs)),
			(std::istreambuf_iterator<char>()));
	const char* c_str_fragment = fragmentShaderSource.c_str();

	glShaderSource(mFragmentShader, 1, &c_str_fragment, nullptr);
	glCompileShader(mFragmentShader);

	glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl << fragmentShaderSource
				<< std::endl;
	}

	mProgram = glCreateProgram();

	glAttachShader(mProgram, mVertexShader);
	glAttachShader(mProgram, mFragmentShader);
	glLinkProgram(mProgram);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);
}

void Shader::init() {
	DE_TRACE()

	initInternal("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
}

void Shader::initDebug() {
	DE_TRACE()

	initInternal("resources/shaders/vertexDebug.shader", "resources/shaders/fragmentDebug.shader");
}

void Shader::use() {
	glUseProgram(mProgram);
};

void Shader::addMatrix(const Matrix4 &matrix, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	//std::cout << "MATRIX LOCATION\n" << matrixLocation << std::endl;
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getData());
};

void Shader::addInt(i32 value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1i(location, value);
};

void Shader::addUInt(u32 value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1ui(location, value);
};

void Shader::addFloat(f32 value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1f(location, value);
};

void Shader::addVector4(Array<f32> *value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform4fv(location, 1, value->getRawData());
};

void Shader::addVector3(Array<f32> *value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3fv(location, 1, value->getRawData());
};

void Shader::addBool(bool value, const String &name) {
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1ui(location, value);
};

}
