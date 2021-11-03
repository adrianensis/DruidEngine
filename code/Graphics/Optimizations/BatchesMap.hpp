#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

class BatchesMap: public ObjectBase
{
    GENERATE_METADATA(BatchesMap)
	PRI_M(SMap(Texture *, Batch *), Batches, NONE);
	PRI_M(bool, IsWorldSpace, GET)

PUB
	~BatchesMap() override;

	void init();
	void render(u32 layer);
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
};