#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"

class Texture;
class Shader;

class Material: public ObjectBase
{
    GENERATE_METADATA(Material)
	PRI_M(Vector4, Color, GET_SET)
	PRI_M(Shader *, Shader, GET_SET);
	PRI_M(Texture *, Texture, GET_SET);
	PRI_M(bool, AlphaEnabled, GET_SET)
	PRI_M(bool, HasBorder, GET_SET)

PUB
	Material();
	void init();
	void bind(bool isWorldSpace);

	SERIALIZE();
	DESERIALIZE();
};