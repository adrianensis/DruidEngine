#ifndef DE_MATERIAL_H
#define DE_MATERIAL_H

#include "DE_Class.h"

namespace DE {

class Texture;
class Shader;

class Material : public DE_Class {

private:
	Shader* mShader;
	Texture* mTexture;

public:

	DE_CLASS(Material, DE_Class);

	void init();

	DE_GET_SET(Shader*, Shader);
	DE_GET_SET(Texture*, Texture);
};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
