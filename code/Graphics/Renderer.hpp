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
	PRI(Animations, NONE, SMap<SStr, Animation *>)
	PRI(RenderereModelMatrix, NONE, Matrix4)
	PRI(PositionOffsetDirty, NONE, bool)
	PRI(ForceRecalculateVertices, NONE, bool)
	PRI(Vertices, NONE, SVec<Vector2>)
	PRI(IsWorldSpace, NONE, bool)

	PRI(CurrentAnimation, GET, Animation *)
	PRI(Color, GETREF_CONST, SArr<f32, 4>)
	PRI(PositionOffset, GET, Vector3)
	PRI(Region, GETREF_CONST_SET, Rectangle)
	PRI(ClipRectangle, GETREF_CONST_SET, Rectangle)
	PRI(Mesh, GET_SET, Mesh *)
	PRI(Material, GET_SET, Material *)
	PRI(InvertAxisX, GET_SET, bool)
	PRI(IsLineMode, GET_SET, bool)
	PRI(Layer, GET_SET, u32)
	PRI(RenderDistance, GET_SET, f32)
	PRI(IsOutOfCamera, GET_SET, bool)
	PRI(IsAlreadyInBatch, GET_SET, bool)
	PRI(Chunk, GET_SET, Chunk *)

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
	CNS SVec<Vector2> &getVertices(bool force = false);
	void forceRecalculateVertices();
	bool hasClipRectangle() CNS {return mClipRectangle.getSize().len() > MathUtils::FLOAT_EPSILON; };

	SERIALIZE();
	DESERIALIZE();
};