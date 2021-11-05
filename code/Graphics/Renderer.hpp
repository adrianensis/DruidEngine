#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Geometry.hpp"
#include "Maths/MathUtils.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Animation/Animation.hpp"

class Material;
class Mesh;
class Chunk;

class Renderer: public Component
{
    GENERATE_METADATA(Renderer)
	
	PUB Renderer();
	PUB ~Renderer() override;
	PUB SERIALIZE();
	PUB DESERIALIZE();
	
	PUB void init() override;

	// Renderer Properties
	PRI Matrix4 mRenderereModelMatrix;
	PRI bool mPositionOffsetDirty = true;
	PRI bool mForceRecalculateVertices = false;
	PRI std::vector<Vector2> mVertices;
	PRI bool mIsWorldSpace = true;
	PRI std::array<f32, 4> mColor; GETREF_CONST(Color)
	PRI Vector3 mPositionOffset; GET(PositionOffset)
	PRI Rectangle mRegion; GETREF_CONST_SET(Region)
	PRI Rectangle mClipRectangle; GETREF_CONST_SET(ClipRectangle)
	PRI Mesh* mMesh = nullptr; GET_SET(Mesh)
	PRI Material* mMaterial = nullptr; GET_SET(Material)
	PRI bool mInvertAxisX = false; GET_SET(InvertAxisX)
	PRI bool mIsLineMode = false; GET_SET(IsLineMode)
	PRI u32 mLayer = 0; GET_SET(Layer)
	PRI f32 mRenderDistance = 0.0f; GET_SET(RenderDistance)
	PRI bool mIsOutOfCamera = false; GET_SET(IsOutOfCamera)
	PRI bool mIsAlreadyInBatch = false; GET_SET(IsAlreadyInBatch)
	PRI Chunk* mChunk = nullptr; GET_SET(Chunk)

	PUB void setColor(const Vector4& color);
	PUB void setPositionOffset (const Vector3& newPositionOffset);
	PUB bool getIsWorldSpace();
	PUB const std::vector<Vector2>& getVertices(bool force = false);
	PUB void forceRecalculateVertices();
	PUB bool hasClipRectangle() const;

	// Animation
	PRI std::map<std::string, Animation> mAnimations; GETREF_CONST(Animations)
	PRI Animation* mCurrentAnimation = nullptr; GET(CurrentAnimation)

	PUB void setAnimation(const std::string& name);
	PUB void addAnimation(const std::string& name, const Animation& animation);
    PUB void removeAnimation(const std::string& name);
	PUB bool hasAnimations() const;
	PUB void updateAnimation();
};