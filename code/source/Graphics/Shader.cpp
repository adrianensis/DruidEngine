#include "Shader.h"
#include "RenderContext.h"
#include "Matrix4.h"
#include "Array.h"
#include <fstream> // std::ifstream
#include <string>
#include <iostream>

namespace DE {

// ---------------------------------------------------------------------------

Shader::Shader() : DE_Class() {
	mVertexShader = -1;
	mFragmentShader = -1;
	mProgram = -1;
};

// ---------------------------------------------------------------------------

Shader::~Shader() = default;

// ---------------------------------------------------------------------------

void Shader::init() {
	mVertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::ifstream vertex_ifs;
	vertex_ifs.open ("resources/shaders/vertex.shader", std::ifstream::in);
	std::string vertexShaderSource((std::istreambuf_iterator<char>(vertex_ifs)), (std::istreambuf_iterator<char>()));

	const char *c_str_vertex = vertexShaderSource.c_str();

	glShaderSource(mVertexShader, 1, &c_str_vertex, nullptr);
	glCompileShader(mVertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl << vertexShaderSource << std::endl;
	}

	mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream fragment_ifs;
	fragment_ifs.open ("resources/shaders/fragment.shader", std::ifstream::in);
	std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragment_ifs)), (std::istreambuf_iterator<char>()));
	const char *c_str_fragment = fragmentShaderSource.c_str();

	glShaderSource(mFragmentShader, 1, &c_str_fragment, nullptr);

	glCompileShader(mFragmentShader);

	mProgram = glCreateProgram();

	glAttachShader(mProgram, mVertexShader);
	glAttachShader(mProgram, mFragmentShader);
	glLinkProgram(mProgram);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
	    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl << vertexShaderSource << std::endl;
	}

	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);

	glBindAttribLocation(mProgram, 0, "position");
}

void Shader::use() {
	glUseProgram(mProgram);
};

// ---------------------------------------------------------------------------

void Shader::addMatrix(const Matrix4& matrix, const std::string& name){
	u32 location = glGetUniformLocation(mProgram, name.c_str());
	//std::cout << "MATRIX LOCATION\n" << matrixLocation << std::endl;
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getData());
};

// ---------------------------------------------------------------------------

void Shader::addInt(i32 value, const std::string& name){
  u32 location = glGetUniformLocation(mProgram, name.c_str());
  glUniform1i(location, value);
};

// ---------------------------------------------------------------------------

void Shader::addUInt(u32 value, const std::string& name){
  u32 location = glGetUniformLocation(mProgram, name.c_str());
  glUniform1ui(location, value);
};

// ---------------------------------------------------------------------------

void Shader::addFloat(f32 value, const std::string& name){
  u32 location = glGetUniformLocation(mProgram, name.c_str());
  glUniform1f(location, value);
};

// ---------------------------------------------------------------------------

void Shader::addVector4(Array<f32>* value, const std::string& name){
  u32 location = glGetUniformLocation(mProgram, name.c_str());
  glUniform4fv(location, 1, value->getRawData());
};

// ---------------------------------------------------------------------------

} /* namespace DE */
