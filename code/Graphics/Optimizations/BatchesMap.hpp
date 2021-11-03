#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

class BatchesMap: public ObjectBase
{
    GENERATE_METADATA(BatchesMap)
	PRI std::map<Texture *, Batch *> mBatches = {};;
	PRI bool mIsWorldSpace = {}; GET(IsWorldSpace)

PUB
	~BatchesMap() override;

	void init();
	void render(u32 layer);
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
};