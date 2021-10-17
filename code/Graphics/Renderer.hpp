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

CLASS(Renderer, Component)
{
	PRI(SMap(SStr, Animation), Animations, GETREF_CONST)
	PRI(Matrix4, RenderereModelMatrix, NONE)
	PRI(bool, PositionOffsetDirty, NONE)
	PRI(bool, ForceRecalculateVertices, NONE)
	PRI(SVec(Vector2), Vertices, NONE)
	PRI(bool, IsWorldSpace, NONE)

	PRI(Animation *, CurrentAnimation, GET)
	PRI(SArr(f32, 4), Color, GETREF_CONST)
	PRI(Vector3, PositionOffset, GET)
	PRI(Rectangle, Region, GETREF_CONST_SET)
	PRI(Rectangle, ClipRectangle, GETREF_CONST_SET)
	PRI(Mesh *, Mesh, GET_SET)
	PRI(Material *, Material, GET_SET)
	PRI(bool, InvertAxisX, GET_SET)
	PRI(bool, IsLineMode, GET_SET)
	PRI(u32, Layer, GET_SET)
	PRI(f32, RenderDistance, GET_SET)
	PRI(bool, IsOutOfCamera, GET_SET)
	PRI(bool, IsAlreadyInBatch, GET_SET)
	PRI(Chunk *, Chunk, GET_SET)

public:
	Renderer();
	~Renderer() OVR;

	void init() OVR;
	void setAnimation(const SStr &name);
	void addAnimation(const SStr &name, Animation animation);
    void removeAnimation(const SStr &name);
	bool hasAnimations() const;
	void updateAnimation();
	void setColor(const Vector4 &color);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsWorldSpace();
	const SVec(Vector2) &getVertices(bool force = false);
	void forceRecalculateVertices();
	bool hasClipRectangle() const {return mClipRectangle.getSize().len() > MathUtils::FLOAT_EPSILON; };

	SERIALIZE();
	DESERIALIZE();
};