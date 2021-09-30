#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Geometry.hpp"
#include "Maths/MathUtils.hpp"
#include "Maths/Matrix4.hpp"


class Material;
class Mesh;
class Animation;
class Chunk;

CLASS(Renderer, Component)
{
	PRI(SMap(SStr, Animation *), Animations, NONE)
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
	void setAnimation(CNS SStr &name);
	void addAnimation(CNS SStr &name, Animation *animation);
	bool hasAnimations() CNS;
	void updateAnimation();
	void setColor(CNS Vector4 &color);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsWorldSpace();
	CNS SVec(Vector2) &getVertices(bool force = false);
	void forceRecalculateVertices();
	bool hasClipRectangle() CNS {return mClipRectangle.getSize().len() > MathUtils::FLOAT_EPSILON; };

	SERIALIZE();
	DESERIALIZE();
};