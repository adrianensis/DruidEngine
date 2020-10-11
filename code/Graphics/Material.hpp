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

	void setAlphaEnabled(bool newAlphaEnabled) {
		mAlphaEnabled = newAlphaEnabled;
	};

	bool getAlphaEnabled() {
		return mAlphaEnabled;
	};

	bool hasBorder() {
		return mHasBorder;
	};

	void setHasBorder(bool border) {
		mHasBorder = border;
	};


};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
