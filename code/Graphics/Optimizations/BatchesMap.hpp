#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

CLASS(BatchesMap, ObjectBase)
{
	PRI_M(SMap(Texture *, Batch *), Batches, NONE);
	PRI_M(bool, IsWorldSpace, GET)

PUB
	~BatchesMap() override;

	void init();
	void render(u32 layer);
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
};