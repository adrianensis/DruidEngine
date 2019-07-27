#include "Material.h"

#include "Texture.h"
#include "Shader.h"

namespace DE {

// ---------------------------------------------------------------------------

Material::Material() : DE_Class(){
  mShader = nullptr;
  mTexture = nullptr;
}

Material::~Material() {
	// TODO Auto-generated destructor stub
}

// ---------------------------------------------------------------------------

Shader* Material::getShader(){ return mShader; }
Texture* Material::getTexture(){ return mTexture; }
void Material::setShader(Shader* shader){ mShader = shader; }
void Material::setTexture(Texture* texture){ mTexture = texture; }

// ---------------------------------------------------------------------------

} /* namespace DE */
