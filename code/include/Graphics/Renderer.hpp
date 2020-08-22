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
	Animation* mCurrentAnimation;
	Material* mMaterial;
	Mesh* mMesh;

	Array<f32>* mColor;

	Vector3 mPositionOffset;
	Matrix4 mPositionOffsetMatrix;

	Vector2 mRegionPosition;
	Vector2 mRegionSize;

	bool mInvertXAxis;

	bool mLineMode;
	bool mAlphaEnabled;
	bool mHasBorder;

	u32 mLayer;

	f32 mRenderDistance;
	bool mOutOfCamera;

	Chunk* mChunk;

	bool mIsAlreadyInBatch;
	bool mIsAffectedByProjection;

public:

	DE_CLASS(Renderer, Component);

	void init() override;

	void setRegion(f32 u, f32 v, f32 width, f32 height);

	/**
	 * Set the animation, by name.
	 * \param string name The name.
	 */
	void setAnimation(const std::string &name);

	/**
	 * Add an animation, by name.
	 * \param string name The name.
	 * \param Animation animation The animation.
	 */
	void addAnimation(const std::string &name, Animation *animation);

	/**
	 * Update de material with information about the region, the alphacolor and the animation.
	 * \param Material material The material.
	 */
	void updateMaterial(Material *material);

	void setColor(const Vector4 &color);

	Vector2 getRegionPosition() const {
		return mRegionPosition;
	};

	Vector2 getRegionSize() const {
		return mRegionSize;
	};

	const Vector3& getPositionOffset() const {
		return mPositionOffset;
	};

	void setPositionOffset(Vector3 newPositionOffset);

	const Matrix4& getPositionOffsetMatrix();

	const Mesh* getMesh() const {
		return mMesh;
	};

	void setMesh(Mesh *newMesh) {
		mMesh = newMesh;
	};


	Material* getMaterial() const {
		return mMaterial;
	};

	void setMaterial(Material *newMaterial) {
		mMaterial = newMaterial;
	};

	void setInvertXAxis(bool newInvertXAxis) {
		mInvertXAxis = newInvertXAxis;
	};

	void setAlphaEnabled(bool newAlphaEnabled) {
		mAlphaEnabled = newAlphaEnabled;
	};

	bool isLineMode() {
		return mLineMode;
	};

	void setLineMode(bool newLineMode) {
		mLineMode = newLineMode;
	};

	bool hasBorder() {
		return mHasBorder;
	};

	void setHasBorder(bool border) {
		mHasBorder = border;
	};

	u32 getLayer() const {
		return mLayer;
	};

	void setLayer(u32 newLayer) {
		mLayer = newLayer;
	};

	f32 getRenderDistance() const {
		return mRenderDistance;
	};

	void setRenderDistance(f32 newRenderDistance) {
		mRenderDistance = newRenderDistance;
	};

	bool isOutOfCamera() const {
		return mOutOfCamera;
	};

	void setIsOutOfCamera(bool newOutOfCamera) {
		mOutOfCamera = newOutOfCamera;
	};

	void setChunk(Chunk *chunk);
	Chunk* getChunk();
	void setIsAlreadyInBatch(bool isAlreadyInBatch) {
		mIsAlreadyInBatch = isAlreadyInBatch;
	};

	bool isAlreadyInBatch() const {
		return mIsAlreadyInBatch;
	};

	bool isAffectedByProjection();

	void renderCollider();
};

} /* namespace DE */

#endif /* DE_RENDERER_H */
