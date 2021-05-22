#include "Graphics/Material.hpp"

#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

Material::Material() {
	mShader = nullptr;
	mTexture = nullptr;
	mAlphaEnabled = true;
	mHasBorder = false;
}

void Material::init() {

}