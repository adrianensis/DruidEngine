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

public:

	DE_CLASS(Material, DE_Class);

	void init();

	Shader* getShader() const {
		return mShader;
	};

	void setShader(Shader *shader) {
		mShader = shader;
	};

	Texture* getTexture() const {
		return mTexture;
	};

	void setTexture(Texture *texture) {
		mTexture = texture;
	};
};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
