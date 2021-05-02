#include "Graphics/Material.hpp"

#include "Memory/Memory.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

namespace DE {

Material::Material() : ObjectBase() {
	mShader = nullptr;
	mTexture = nullptr;
	mAlphaEnabled = true;
	mHasBorder = false;
}

Material::~Material() {
	// Memory::free(mShader);
}

void Material::init() {
TRACE()
}

}
