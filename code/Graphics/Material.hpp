#ifndef DE_MATERIAL_H
#define DE_MATERIAL_H

#include "DE_Class.hpp"
#include "Vector4.hpp"

namespace DE {

class Texture;
class Shader;

class Material: public DE_Class {

private:
	Shader* mShader;
	Texture* mTexture;
	Vector4 mColor;
	bool mAlphaEnabled;
	bool mHasBorder;

public:

	DE_CLASS(Material, DE_Class)

	DE_GET_SET(Shader)
	DE_GET_SET(Texture)
	DE_GET_SET(AlphaEnabled)
	DE_GET_SET(HasBorder)

	void init();
};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
