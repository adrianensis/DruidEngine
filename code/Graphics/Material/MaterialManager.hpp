#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

#include <map>

class Material;
class Texture;

CLASS(MaterialManager, ObjectBase), SINGLETON(MaterialManager)
{
	PRI(TexturesMap, NONE, std::map<std::string, class Texture *>);
	PRI(MaterialsMap, NONE, std::map<std::string, class Material *>);
	PRI(NoTextureMaterial, NONE, Material *);

public:
	~MaterialManager() override;

	void init();
	Texture *loadTexture(const std::string &path);
	Material *loadMaterial(const std::string &path);
	Material *loadNoTextureMaterial();
};