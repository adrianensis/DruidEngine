#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"
#include "Graphics/Chunk.hpp"

namespace DE {

class Renderer : public Component {
private:

	 HashMap<String, Animation*>* mAnimations;
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

	GET(CurrentAnimation);
	GET(Color);
	GET(PositionOffset);
	GET(RegionPosition);
	GET(RegionSize);
	GET_SET(Mesh);
	GET_SET(Material);
	GET_SET(IsInvertAxis);
	GET_SET(IsLineMode);
	GET_SET(Layer);
	GET_SET(RenderDistance);
	GET_SET(IsOutOfCamera);
	GET_SET(IsAlreadyInBatch);
	GET_SET(Chunk);

	GENERATE_METADATA(Renderer);

	Renderer();
	virtual ~Renderer() override;

	void init() override;
	void setAnimation(StringRef name);
	void addAnimation(StringRef name, Animation* animation);
	bool hasAnimations() const;
	void updateAnimation();
	void setColor(const Vector4& color);
	void setRegion(f32 u, f32 v, f32 width, f32 height);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsAffectedByProjection();
	const Array<Vector2>* getVertices(bool force = false);
	void forceRecalculateVertices();

	virtual void save(ConfigMap* configMap, StringRef objectName) override;
	virtual void load(ConfigMap* configMap, StringRef objectName) override;
};
}

