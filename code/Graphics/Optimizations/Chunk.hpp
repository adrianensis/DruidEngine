#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"


class Renderer;
class BatchesMap;

CLASS(Chunk, ObjectBase)
{
	PRI(SLst(Renderer *) *, Renderers, NONE);
	PRI(Vector3, LeftTop, NONE)
	PRI(f32, Size, NONE)
	PRI(bool, IsLoaded, GET)
	PRI(Vector3, Center, GET)
	PRI(f32, Radius, GET)

public:
	~Chunk() OVR;
	void init();
	void set(CNS Vector3 &leftTop, f32 size);
	void load();
	void update(BatchesMap * batchesMap);
	void unload();
	bool containsRenderer(CNS Renderer *renderer, f32 epsilon = 0.0f) CNS;
	bool containsRendererSphere(CNS Renderer *renderer) CNS;
	void addRenderer(Renderer * renderer);
};