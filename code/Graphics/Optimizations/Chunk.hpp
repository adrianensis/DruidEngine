#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector3.hpp"

#include <list>

class Renderer;
class BatchesMap;

CLASS(Chunk, ObjectBase)
{
	PRI(Renderers, NONE, std::list<Renderer *> *);
	PRI(LeftTop, NONE, Vector3)
	PRI(Size, NONE, f32)
	PRI(IsLoaded, GET, bool)
	PRI(Center, GET, Vector3)
	PRI(Radius, GET, f32)

public:
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