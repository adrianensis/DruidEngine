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
	PRI_M(Matrix4, RenderereModelMatrix, NONE)
	PRI_M(bool, PositionOffsetDirty, NONE)
	PRI_M(bool, ForceRecalculateVertices, NONE)
	PRI_M(SVec(Vector2), Vertices, NONE)
	PRI_M(bool, IsWorldSpace, NONE)
	PRI_M(SArr(f32, 4), Color, GETREF_CONST)
	PRI_M(Vector3, PositionOffset, GET)
	PRI_M(Rectangle, Region, GETREF_CONST_SET)
	PRI_M(Rectangle, ClipRectangle, GETREF_CONST_SET)
	PRI_M(Mesh*, Mesh, GET_SET)
	PRI_M(Material*, Material, GET_SET)
	PRI_M(bool, InvertAxisX, GET_SET)
	PRI_M(bool, IsLineMode, GET_SET)
	PRI_M(u32, Layer, GET_SET)
	PRI_M(f32, RenderDistance, GET_SET)
	PRI_M(bool, IsOutOfCamera, GET_SET)
	PRI_M(bool, IsAlreadyInBatch, GET_SET)
	PRI_M(Chunk*, Chunk, GET_SET)

	PUB void setColor(const Vector4& color);
	PUB void setPositionOffset (const Vector3& newPositionOffset);
	PUB bool getIsWorldSpace();
	PUB const SVec(Vector2)& getVertices(bool force = false);
	PUB void forceRecalculateVertices();
	PUB bool hasClipRectangle() const;

	// Animation
	PRI_M(SMap(SStr, Animation), Animations, GETREF_CONST)
	PRI_M(Animation*, CurrentAnimation, GET)

	PUB void setAnimation(const SStr& name);
	PUB void addAnimation(const SStr& name, const Animation& animation);
    PUB void removeAnimation(const SStr& name);
	PUB bool hasAnimations() const;
	PUB void updateAnimation();
};