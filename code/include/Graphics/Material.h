#ifndef DE_MATERIAL_H
#define DE_MATERIAL_H

#include "Texture.h"
#include "Shader.h"

namespace DE {

class Material : public DE_Class {
private:
	Shader* mShader;
	Texture* mTexture;
public:
	Material();
	~Material() override;
};

} /* namespace DE */

#endif /* DE_MATERIAL_H */
