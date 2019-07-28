#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.h"

#include <string>

namespace DE {

class Material;
class Mesh;
class Animation;
template <class K, class V> class HashMap;

class Renderer : public Component{

private:

	HashMap<std::string, Animation*>* mAnimations;
	Animation* mCurrentAnimation;
	Material* mMaterial;
	Mesh* mMesh;

public:

	DE_CLASS(Renderer);

	virtual void init() override;

	void setAnimation(std::string name);
	void addAnimation(std::string name, Animation* animation);
	void updateMaterial(Material* material);

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setMaterial(Material* material);
	Material* getMaterial();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
