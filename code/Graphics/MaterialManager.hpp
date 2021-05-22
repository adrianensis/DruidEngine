#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

#include <map>

class Material;

CLASS(MaterialManager, ObjectBase) , SINGLETON(MaterialManager) {

	PRI(TexturesMap, NONE, std::map<std::string, class Texture*>*);
	PRI(MaterialsMap, NONE, std::map<std::string, class Material*>*);
	PRI(NoTextureMaterial, NONE, Material*);

public:

	virtual ~MaterialManager() override;

	void init();
	class Texture* loadTexture(const std::string& path);
	class Material* loadMaterial(const std::string& path);
	class Material* loadNoTextureMaterial();
};