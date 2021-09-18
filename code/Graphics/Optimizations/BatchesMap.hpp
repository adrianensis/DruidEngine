#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

CLASS(BatchesMap, ObjectBase)
{
	PRI(Batches, NONE, SMap<Texture *, Batch *>);
	PRI(IsWorldSpace, GET, bool)

public:
	~BatchesMap() OVR;

	void init();
	void render(u32 layer);
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
};