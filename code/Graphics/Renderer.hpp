#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Geometry.hpp"
#include "Maths/MathUtils.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Optimizations/Chunk.hpp"

#include <vector>
#include <array>
#include <map>

CLASS(Renderer, Component)
{
	PRI(Animations, NONE, std::map<std::string, Animation *>)
	PRI(RenderereModelMatrix, NONE, Matrix4)
	PRI(PositionOffsetDirty, NONE, bool)
	PRI(ForceRecalculateVertices, NONE, bool)
	PRI(Vertices, NONE, std::vector<Vector2>)
	PRI(IsWorldSpace, NONE, bool)

	PRI(CurrentAnimation, GET, Animation *)
	PRI(Color, GETREF_CONST, std::array<f32, 4>)
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
	~Renderer() override;

	void init() override;
	void setAnimation(const std::string &name);
	void addAnimation(const std::string &name, Animation *animation);
	bool hasAnimations() const;
	void updateAnimation();
	void setColor(const Vector4 &color);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsWorldSpace();
	const std::vector<Vector2> &getVertices(bool force = false);
	void forceRecalculateVertices();
	bool hasClipRectangle() const {return mClipRectangle.getSize().len() > MathUtils::FLOAT_EPSILON; };

	virtual void serialize(JSON &json) const override;
	virtual void deserialize(const JSON &json) override;
};