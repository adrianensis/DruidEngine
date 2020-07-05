#ifndef DE_SHADER_H
#define DE_SHADER_H

#include "DE_Class.hpp"
#include "BasicTypes.hpp"
#include "Vector4.hpp"

namespace DE {

template<class T> class Array;

class Matrix4;

// ---------------------------------------------------------------------------

class Shader: public DE_Class {
private:
	u32 mVertexShader;
	u32 mFragmentShader;
	u32 mProgram;

	static Shader* msShaderDefault;
	static Shader* msShaderDebug;

	void initInternal(const std::string &vertex, const std::string &fragment);

public:

	DE_CLASS(Shader, DE_Class)
	;

	static Shader* getDefaultShader();
	static Shader* getDebugShader();

	static void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(const Matrix4 &matrix, const std::string &name);
	void addInt(i32 value, const std::string &name);
	void addUInt(u32 value, const std::string &name);
	void addFloat(f32 value, const std::string &name);
	void addVector4(Array<f32> *value, const std::string &name);
	void addVector3(Array<f32> *value, const std::string &name);
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* DE_SHADER_H */
