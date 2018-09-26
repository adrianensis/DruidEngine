#ifndef DE_SHADER_H
#define DE_SHADER_H

#include "DE_Class.h"
#include "BasicTypes.h"

namespace DE {

class Shader : public DE_Class{
private:
	u32 mVertexShader;
	u32 mFragmentShader;
	u32 mProgram;


public:
	Shader();
	~Shader() override;

	void init();
	void use();
};

} /* namespace DE */

#endif /* DE_SHADER_H */
