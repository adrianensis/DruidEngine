#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"


class Material;
class Texture;

CLASS(MaterialManager, ObjectBase), SINGLETON(MaterialManager)
{
	PRI(SMap(SStr, class Texture *), TexturesMap, NONE);
	PRI(SMap(SStr, class Material *), MaterialsMap, NONE);
	PRI(Material *, NoTextureMaterial, NONE);

public:
	~MaterialManager() OVR;

	void init();
	Texture *loadTexture(const SStr &path);
	Material *loadMaterial(const SStr &path);
	Material *loadNoTextureMaterial();
};