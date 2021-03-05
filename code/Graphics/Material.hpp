#pragma once

#include "Core/DE_Class.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

class Texture;
class Shader;

class Material: public DE_Class {

private:
	Vector4 mColor;

	DE_M_GET_SET(Shader, Shader*)
	DE_M_GET_SET(Texture, Texture*)
	DE_M_GET_SET(AlphaEnabled, bool)
	DE_M_GET_SET(HasBorder, bool)

public:

	DE_CLASS_BODY(Material)

	void init();
};

}

