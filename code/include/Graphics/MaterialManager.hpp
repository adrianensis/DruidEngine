#ifndef DE_MATERIALMANAGER_H_
#define DE_MATERIALMANAGER_H_

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include <string>

namespace DE {

template<class K, class V> class HashMap;
class Texture;
class Material;

class MaterialManager: public DE_Class, public Singleton<MaterialManager> {

	HashMap<std::string, Texture*>* mTexturesMap;
	HashMap<std::string, Material*>* mMaterialsMap;

public:

	DE_CLASS(MaterialManager, DE_Class)
	;

	void init();
	Texture* loadTexture(const std::string &path);
	Material* loadMaterial(const std::string &path);
};

} /* namespace DE */

#endif /* DE_MATERIALMANAGER_H_ */
