#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"


class Material;
class Texture;

CLASS(MaterialManager, ObjectBase), SINGLETON(MaterialManager)
{
	PRI_M(SMap(SStr, class Texture *), TexturesMap, NONE);
	PRI_M(SMap(SStr, class Material *), MaterialsMap, NONE);
	PRI_M(Material *, NoTextureMaterial, NONE);

PUB
	~MaterialManager() override;

	void init();
	Texture *loadTexture(const SStr &path);
	Material *loadMaterial(const SStr &path);
	Material *loadNoTextureMaterial();
};