#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"


class Renderer;
class BatchesMap;

class Chunk: public ObjectBase
{
    GENERATE_METADATA(Chunk)
	PRI_M(SLst(Renderer *) *, Renderers, NONE);
	PRI_M(Vector3, LeftTop, NONE)
	PRI_M(f32, Size, NONE)
	PRI_M(bool, IsLoaded, GET)
	PRI_M(Vector3, Center, GET)
	PRI_M(f32, Radius, GET)

PUB
	~Chunk() override;
	void init();
	void set(const Vector3 &leftTop, f32 size);
	void load();
	void update(BatchesMap * batchesMap);
	void unload();
	bool containsRenderer(const Renderer *renderer, f32 epsilon = 0.0f) const;
	bool containsRendererSphere(const Renderer *renderer) const;
	void addRenderer(Renderer * renderer);
};