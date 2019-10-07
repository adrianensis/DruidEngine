#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.h"
#include "Vector2.h"
#include "Vector4.h"

#include <string>

namespace DE {

class Material;
class Mesh;
class Animation;
template <class K, class V> class HashMap;
template <class T> class Array;

class Renderer : public Component{

private:

	HashMap<std::string, Animation*>* mAnimations;
	Animation* mCurrentAnimation;
	Material* mMaterial;
	Mesh* mMesh;

	Array<f32>* mColor;

	Vector2 mRegionPosition;
  f32 mRegionWidth;
  f32 mRegionHeight;

	bool mInvertXAxis;

	bool mLineMode;
	bool mAlphaEnabled;

public:

	DE_CLASS(Renderer, Component);

	void init() override;

	void setRegion(f32 u, f32 v, f32 width, f32 height);

	void setAnimation(const std::string& name);
	void addAnimation(const std::string& name, Animation* animation);
	void updateMaterial(Material* material);

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setMaterial(Material* material);
	Material* getMaterial();

	void setColor(const Vector4& color);

	void setInvertXAxis(bool invertXAxis);
	void setAlphaEnabled(bool alphaEnabled);

	void setLineMode(bool lineMode);
	bool isLineMode();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
