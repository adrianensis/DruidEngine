#include "Material.h"

#include "Memory.h"
#include "Texture.h"
#include "Shader.h"

namespace DE {

// ---------------------------------------------------------------------------

Material::Material() : DE_Class(){
  mShader = nullptr;
  mTexture = nullptr;
}

Material::~Material() {
  Memory::free<Shader>(mShader);
}

void Material::init(){
  mShader = Memory::allocate<Shader>(); // TODO : Share the same shader !!
	mShader->init();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
