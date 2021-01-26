#include "Graphics/Material.hpp"

#include "Memory/Memory.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Material::Material() : DE_Class() {
	mShader = nullptr;
	mTexture = nullptr;
	mAlphaEnabled = true;
	mHasBorder = false;
}

Material::~Material() {
	// DE_FREE(mShader);
}

void Material::init() {
DE_TRACE()
}

// ---------------------------------------------------------------------------

} /* namespace DE */
