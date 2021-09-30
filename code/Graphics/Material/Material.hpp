#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"

class Texture;
class Shader;

CLASS(Material, ObjectBase)
{
	PRI(Vector4, Color, GET_SET)
	PRI(Shader *, Shader, GET_SET);
	PRI(Texture *, Texture, GET_SET);
	PRI(bool, AlphaEnabled, GET_SET)
	PRI(bool, HasBorder, GET_SET)

public:
	Material();
	void init();
	void bind(bool isWorldSpace);

	SERIALIZE();
	DESERIALIZE();
};