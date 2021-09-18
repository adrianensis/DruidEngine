#pragma once

#include "Core/Core.hpp"
#include "Core/Singleton.hpp"


class Material;
class Texture;

CLASS(MaterialManager, ObjectBase), SINGLETON(MaterialManager)
{
	PRI(TexturesMap, NONE, SMap<SStr, class Texture *>);
	PRI(MaterialsMap, NONE, SMap<SStr, class Material *>);
	PRI(NoTextureMaterial, NONE, Material *);

public:
	~MaterialManager() OVR;

	void init();
	Texture *loadTexture(CNS SStr &path);
	Material *loadMaterial(CNS SStr &path);
	Material *loadNoTextureMaterial();
};