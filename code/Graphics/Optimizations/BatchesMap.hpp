#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

CLASS(BatchesMap, ObjectBase)
{
	PRI(SMap(Texture *, Batch *), Batches, NONE);
	PRI(bool, IsWorldSpace, GET)

public:
	~BatchesMap() OVR;

	void init();
	void render(u32 layer);
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
};