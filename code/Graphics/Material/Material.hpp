#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector4.hpp"

class Texture;
class Shader;

class Material: public ObjectBase
{
    GENERATE_METADATA(Material)
	PRI Vector4 mColor = {}; GET_SET(Color)
	PRI Shader * mShader = {}; GET_SET(Shader);
	PRI Texture * mTexture = {}; GET_SET(Texture);
	PRI bool mAlphaEnabled = {}; GET_SET(AlphaEnabled)
	PRI bool mHasBorder = {}; GET_SET(HasBorder)

PUB
	Material();
	void init();
	void bind(bool isWorldSpace);

	SERIALIZE();
	DESERIALIZE();
};