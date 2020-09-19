#include "Material.hpp"

#include "Memory.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Material::Material() : DE_Class() {
	mShader = nullptr;
	mTexture = nullptr;
	mAlphaEnabled = true;
	mHasBorder = false;
}

Material::~Material() {
	// Memory::free<Shader>(mShader);
}

void Material::init() {
DE_TRACE()
}

// ---------------------------------------------------------------------------

} /* namespace DE */
