#include "Material.hpp"

#include "Memory.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Material::Material() :
		DE_Class() {
	mShader = nullptr;
	mTexture = nullptr;
}

Material::~Material() {
	// Memory::free<Shader>(mShader);
}

void Material::init() {
TRACE()
}

// ---------------------------------------------------------------------------

} /* namespace DE */
