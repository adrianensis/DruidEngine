#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

class Texture;
class Shader;

class Material : public ObjectBase {

private:
	Vector4 mColor;

	 Shader* mShader;
	 Texture* mTexture;
	 bool mAlphaEnabled;
	 bool mHasBorder;

public:

	GENERATE_METADATA(Material);

	Material();
	virtual ~Material() override;;

	GET_SET(Shader);
	GET_SET(Texture);
	GET_SET(AlphaEnabled);
	GET_SET(HasBorder);

	void init();
};
}

