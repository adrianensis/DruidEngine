#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.h"
#include "Vector2.h"

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

	Vector2 mRegionPosition;
  f32 mRegionWidth;
  f32 mRegionHeight;

public:

	DE_CLASS(Renderer);

	virtual void init() override;

	void setRegion(f32 u, f32 v, f32 width, f32 height);

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
