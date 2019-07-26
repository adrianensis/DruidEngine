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

	DE_CLASS(Material);

	Shader* getShader();
	Texture* getTexture();

	void setShader(Shader* shader);
	void setTexture(Texture* texture);
};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
