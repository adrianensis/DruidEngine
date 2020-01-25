#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

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

	Vector3 mPositionOffset;
	Matrix4* mPositionOffsetMatrix;

	Vector2 mRegionPosition;
  f32 mRegionWidth;
  f32 mRegionHeight;

	bool mInvertXAxis;

	bool mLineMode;
	bool mAlphaEnabled;

	u32 mLayer;

	f32 mRenderDistance;
	bool mOutOfCamera;

	bool mIsInChunk;

public:

	DE_CLASS(Renderer, Component);

	void init() override;

	void setRegion(f32 u, f32 v, f32 width, f32 height);

	/**
	* Set the animation, by name.
	* \param string name The name.
	*/
	void setAnimation(const std::string& name);

	/**
	* Add an animation, by name.
	* \param string name The name.
	* \param Animation animation The animation.
	*/
	void addAnimation(const std::string& name, Animation* animation);

	/**
	* Update de material with information about the region, the alphacolor and the animation.
	* \param Material material The material.
	*/
	void updateMaterial(Material* material);

	void setColor(const Vector4& color);
	bool isLineMode();

	DE_GET_SET(Vector3, PositionOffset);

	const Matrix4& getPositionOffsetMatrix();

	DE_GET_SET(Mesh*, Mesh);

	DE_GET_SET(Material*, Material);

	DE_SET(bool, InvertXAxis);
	DE_SET(bool, AlphaEnabled);
	DE_SET(bool, LineMode);

	DE_GET_SET(u32, Layer);
	DE_GET_SET(f32, RenderDistance);
	DE_GET_SET(bool, OutOfCamera);
	DE_GET_SET(bool, IsInChunk);
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
