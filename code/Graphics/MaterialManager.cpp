#include "Graphics/MaterialManager.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Shader.hpp"

MaterialManager::~MaterialManager() {
	FOR_MAP(it, *mTexturesMap) {
		delete it->second;
	}

	delete mTexturesMap;

	FOR_MAP(it, *mMaterialsMap) {
		delete it->second;
	}

	delete mMaterialsMap;

	delete mNoTextureMaterial;

	Shader::freeStaticShaders();
}

void MaterialManager::init() {
	TRACE()

	mTexturesMap = new std::map<std::string, Texture*>;
	mMaterialsMap = new std::map<std::string, Material*>;
}

Texture* MaterialManager::loadTexture(const std::string& path) {
	Texture* texture = nullptr;

	if (MAP_CONTAINS(*mTexturesMap, path)) {
		texture = mTexturesMap->at(path);
	} else {
		texture = new Texture;
		texture->init(path);
		MAP_INSERT(*mTexturesMap, path, texture);
	}

	return texture;
}

Material* MaterialManager::loadMaterial(const std::string& path) {
	Material* material = nullptr;

	if (MAP_CONTAINS(*mMaterialsMap, path)) {
		material = mMaterialsMap->at(path);
	} else {
		material = new Material;
		material->init();
		material->setTexture(loadTexture(path));
		material->setShader(Shader::getDefaultShader());
		MAP_INSERT(*mMaterialsMap, path, material);
	}

	return material;
}

Material* MaterialManager::loadNoTextureMaterial() {

	if(!mNoTextureMaterial){
		mNoTextureMaterial = new Material;
		mNoTextureMaterial->init();
		mNoTextureMaterial->setShader(Shader::getDefaultShader());
	}

	return mNoTextureMaterial;
}