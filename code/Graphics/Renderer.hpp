#ifndef DE_RENDERER_H
#define DE_RENDERER_H

#include "Component.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Animation.hpp"
#include "HashMap.hpp"
#include "Array.hpp"
#include "Chunk.hpp"

#include <string>

namespace DE {

class Renderer: public Component {
private:

	using AnimationsMap = HashMap<std::string, Animation*>;
	DE_M(Animations, AnimationsMap*)
	DE_M(RenderereModelMatrix, Matrix4)
	DE_M(PositionOffsetDirty, bool)
	DE_M(ForceRecalculateVertices, bool)
	DE_M(Vertices, Array<Vector2>*)
	DE_M(IsAffectedByProjection, bool)

	DE_M_GET(CurrentAnimation, Animation*)
	DE_M_GET(Color, Array<f32>*)
	DE_M_GET(PositionOffset, Vector3)
	DE_M_GET(RegionPosition, Vector2)
	DE_M_GET(RegionSize, Vector2)
	DE_M_GET_SET(Mesh, Mesh*)
	DE_M_GET_SET(Material, Material*)
	DE_M_GET_SET(IsInvertAxis, bool)
	DE_M_GET_SET(IsLineMode, bool)
	DE_M_GET_SET(Layer, u32)
	DE_M_GET_SET(RenderDistance, f32)
	DE_M_GET_SET(IsOutOfCamera, bool)
	DE_M_GET_SET(IsAlreadyInBatch, bool)
	DE_M_GET_SET(Chunk, Chunk*)

public:

	DE_CLASS(Renderer, Component)

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
