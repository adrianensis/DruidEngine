#pragma once

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include <string>

namespace DE {

template<class K, class V> class HashMap;
class Texture;
class Material;

class MaterialManager: public DE_Class, public Singleton<MaterialManager> {

	using PathTextureMap = HashMap<String, Texture*>;
	DE_M(TexturesMap, PathTextureMap*)
	using PathMaterialMap = HashMap<String, Material*>;
	DE_M(MaterialsMap, PathMaterialMap*)
	DE_M(NoTextureMaterial, Material*)

public:

	DE_CLASS_BODY(MaterialManager)

	void init();
	Texture* loadTexture(const String &path);
	Material* loadMaterial(const String &path);

	Material* loadNoTextureMaterial();
};

}

