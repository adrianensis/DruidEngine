#ifndef DE_MATERIALMANAGER_H_
#define DE_MATERIALMANAGER_H_

#include "Core/DE_Class.hpp"
#include "Core/Singleton.hpp"
#include <string>

namespace DE {

template<class K, class V> class HashMap;
class Texture;
class Material;

class MaterialManager: public DE_Class, public Singleton<MaterialManager> {

	using PathTextureMap = HashMap<std::string, Texture*>;
	DE_M(TexturesMap, PathTextureMap*)
	using PathMaterialMap = HashMap<std::string, Material*>;
	DE_M(MaterialsMap, PathMaterialMap*)
	DE_M(NoTextureMaterial, Material*)

public:

	DE_CLASS(MaterialManager)

	void init();
	Texture* loadTexture(const std::string &path);
	Material* loadMaterial(const std::string &path);

	Material* loadNoTextureMaterial();
};

} /* namespace DE */

#endif /* DE_MATERIALMANAGER_H_ */
