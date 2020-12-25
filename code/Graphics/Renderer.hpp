#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"

#include <string>

namespace DE {

class Material;
class Mesh;
class Animation;
class Chunk;
template<class K, class V> class HashMap;
template<class T> class Array;

class Renderer: public Component {
private:

	HashMap<std::string, Animation*>* mAnimations;
	Matrix4 mRenderereModelMatrix;
	bool mPositionOffsetDirty;
	bool mForceRecalculateVertices;
	Array<Vector2>* mVertices;
	bool mIsAffectedByProjection;

	Animation* mCurrentAnimation;
	Array<f32>* mColor;
	Vector3 mPositionOffset;
	Vector2 mRegionPosition;
	Vector2 mRegionSize;
	Mesh* mMesh;
	Material* mMaterial;
	bool mIsInvertAxis;
	bool mIsLineMode;
	u32 mLayer;
	f32 mRenderDistance;
	bool mIsOutOfCamera;
	bool mIsAlreadyInBatch;
	Chunk* mChunk;

public:

	DE_CLASS(Renderer, Component);

	DE_GET(CurrentAnimation)
	DE_GET(Color)
	DE_GET(PositionOffset)
	DE_GET(RegionPosition)
	DE_GET(RegionSize)
	DE_GET_SET(Mesh)
	DE_GET_SET(Material)
	DE_GET_SET(IsInvertAxis)
	DE_GET_SET(IsLineMode)
	DE_GET_SET(Layer)
	DE_GET_SET(RenderDistance)
	DE_GET_SET(IsOutOfCamera)
	DE_GET_SET(IsAlreadyInBatch)
	DE_GET_SET(Chunk)

	void init() override;
	void setAnimation(const std::string& name);
	void addAnimation(const std::string& name, Animation* animation);
	bool hasAnimations() const;
	void updateAnimation();
	void setColor(const Vector4& color);
	void setRegion(f32 u, f32 v, f32 width, f32 height);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsAffectedByProjection();
	const Array<Vector2>* getVertices(bool force = false);
	void forceRecalculateVertices();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
