#include "Graphics/MaterialManager.hpp"
#include "Containers/HashMap.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Shader.hpp"

namespace DE {

// ---------------------------------------------------------------------------

MaterialManager::MaterialManager() : DE_Class() {
	mTexturesMap = nullptr;
	mMaterialsMap = nullptr;
	mNoTextureMaterial = nullptr;
}

MaterialManager::~MaterialManager() {
	FOR_LIST(it, mTexturesMap->getValues()) {
		Memory::free<Texture>(it.get());
	}

	Memory::free<HashMap<std::string, Texture*>>(mTexturesMap);

	FOR_LIST(it, mMaterialsMap->getValues()) {
		Memory::free<Material>(it.get());
	}

	Memory::free<HashMap<std::string, Material*>>(mMaterialsMap);

	Memory::free<Material>(mNoTextureMaterial);

	Shader::freeStaticShaders();
}

// ---------------------------------------------------------------------------

void MaterialManager::init() {
	DE_TRACE()

	mTexturesMap = Memory::allocate<HashMap<std::string, Texture*>>();
	mTexturesMap->init();

	mMaterialsMap = Memory::allocate<HashMap<std::string, Material*>>();
	mMaterialsMap->init();
}

// ---------------------------------------------------------------------------

Texture* MaterialManager::loadTexture(const std::string &path) {
	Texture* texture = nullptr;

	if (mTexturesMap->contains(path)) {
		texture = mTexturesMap->get(path);
	} else {
		texture = Memory::allocate<Texture>();
		texture->init(path);
		mTexturesMap->set(path, texture);
	}

	return texture;
}

// ---------------------------------------------------------------------------

Material* MaterialManager::loadMaterial(const std::string &path) {
	Material* material = nullptr;

	if (mMaterialsMap->contains(path)) {
		material = mMaterialsMap->get(path);
	} else {
		material = Memory::allocate<Material>();
		material->init();
		material->setTexture(loadTexture(path));
		material->setShader(Shader::getDefaultShader());
		mMaterialsMap->set(path, material);
	}

	return material;
}

// ---------------------------------------------------------------------------

Material* MaterialManager::loadNoTextureMaterial() {

	if(!mNoTextureMaterial){
		mNoTextureMaterial = Memory::allocate<Material>();
		mNoTextureMaterial->init();
		mNoTextureMaterial->setShader(Shader::getDefaultShader());
	}

	return mNoTextureMaterial;
}

} /* namespace DE */
