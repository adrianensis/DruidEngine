#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"


class Renderer;
class BatchesMap;

CLASS(Chunk, ObjectBase)
{
	PRI(Renderers, NONE, SLst<Renderer *> *);
	PRI(LeftTop, NONE, Vector3)
	PRI(Size, NONE, f32)
	PRI(IsLoaded, GET, bool)
	PRI(Center, GET, Vector3)
	PRI(Radius, GET, f32)

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