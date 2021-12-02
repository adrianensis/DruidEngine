#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"

class MaterialManager: public ObjectBase, public Singleton<MaterialManager>
{
	GENERATE_METADATA(MaterialManager)
	PRI std::map<std::string, class Texture *> mTexturesMap;
	PRI std::map<std::string, class Material *> mMaterialsMap;
	PRI Material* mNoTextureMaterial = nullptr;

PUB
	~MaterialManager() override;

	void init();
	Texture *loadTexture(const std::string &path);
	Material *loadMaterial(const std::string &path);
	Material *loadNoTextureMaterial();
};