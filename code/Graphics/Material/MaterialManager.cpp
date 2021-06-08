#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Shader.hpp"

MaterialManager::~MaterialManager() {

	MAP_DELETE_CONTENT(mTexturesMap)
	MAP_DELETE_CONTENT(mMaterialsMap)

	if(mNoTextureMaterial) {
		DELETE(mNoTextureMaterial);
	}

	Shader::freeStaticShaders();
}

void MaterialManager::init() {
	TRACE()
}

Texture* MaterialManager::loadTexture(const std::string& path) {
	Texture* texture = nullptr;

	if (MAP_CONTAINS(mTexturesMap, path)) {
		texture = mTexturesMap.at(path);
	} else {
		texture = NEW(Texture);
		texture->init(path);
		MAP_INSERT(mTexturesMap, path, texture);
	}

	return texture;
}

Material* MaterialManager::loadMaterial(const std::string& path) {
	Material* material = nullptr;

	if (MAP_CONTAINS(mMaterialsMap, path)) {
		material = mMaterialsMap.at(path);
	} else {
		material = NEW(Material);
		material->init();
		material->setTexture(loadTexture(path));
		material->setShader(Shader::getDefaultShader());
		MAP_INSERT(mMaterialsMap, path, material);
	}

	return material;
}

Material* MaterialManager::loadNoTextureMaterial() {

	if(!mNoTextureMaterial){
		mNoTextureMaterial = NEW(Material);
		mNoTextureMaterial->init();
		mNoTextureMaterial->setShader(Shader::getDefaultShader());
	}

	return mNoTextureMaterial;
}