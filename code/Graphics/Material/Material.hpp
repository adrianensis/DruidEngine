#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector4.hpp"

class Texture;
class Shader;

CLASS(Material, ObjectBase)
{
	PRI(Color, GET_SET, Vector4)
	PRI(Shader, GET_SET, Shader *);
	PRI(Texture, GET_SET, Texture *);
	PRI(AlphaEnabled, GET_SET, bool)
	PRI(HasBorder, GET_SET, bool)

public:
	Material();
	void init();
	void bind(bool isWorldSpace);
};