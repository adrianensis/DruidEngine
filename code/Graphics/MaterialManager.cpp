#include "Graphics/MaterialManager.hpp"
#include "Containers/HashMap.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Shader.hpp"

namespace DE {

MaterialManager::MaterialManager() : DE_Class() {
	mTexturesMap = nullptr;
	mMaterialsMap = nullptr;
	mNoTextureMaterial = nullptr;
}

MaterialManager::~MaterialManager() {
	FOR_LIST(it, mTexturesMap->getValues()) {
		DE_FREE(it.get());
	}

	DE_FREE(mTexturesMap);

	FOR_LIST(it, mMaterialsMap->getValues()) {
		DE_FREE(it.get());
	}

	DE_FREE(mMaterialsMap);

	DE_FREE(mNoTextureMaterial);

	Shader::freeStaticShaders();
}

void MaterialManager::init() {
	DE_TRACE()

	mTexturesMap = DE_NEW<HashMap<std::string, Texture*>>();
	mTexturesMap->init();

	mMaterialsMap = DE_NEW<HashMap<std::string, Material*>>();
	mMaterialsMap->init();
}

Texture* MaterialManager::loadTexture(const std::string &path) {
	Texture* texture = nullptr;

	if (mTexturesMap->contains(path)) {
		texture = mTexturesMap->get(path);
	} else {
		texture = DE_NEW<Texture>();
		texture->init(path);
		mTexturesMap->set(path, texture);
	}

	return texture;
}

Material* MaterialManager::loadMaterial(const std::string &path) {
	Material* material = nullptr;

	if (mMaterialsMap->contains(path)) {
		material = mMaterialsMap->get(path);
	} else {
		material = DE_NEW<Material>();
		material->init();
		material->setTexture(loadTexture(path));
		material->setShader(Shader::getDefaultShader());
		mMaterialsMap->set(path, material);
	}

	return material;
}

Material* MaterialManager::loadNoTextureMaterial() {

	if(!mNoTextureMaterial){
		mNoTextureMaterial = DE_NEW<Material>();
		mNoTextureMaterial->init();
		mNoTextureMaterial->setShader(Shader::getDefaultShader());
	}

	return mNoTextureMaterial;
}

}
