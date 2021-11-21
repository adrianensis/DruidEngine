#pragma once

#include "Core/Core.hpp"


class Texture;
class Renderer;
class Batch;

class BatchesMap: public ObjectBase
{
    GENERATE_METADATA(BatchesMap)
	PRI std::map<Texture *, Batch *> mBatchesDynamic;
	PRI std::map<Texture *, Batch *> mBatchesDynamicScreenSpace;
	PRI std::map<Texture *, Batch *> mBatchesStatic;
	PRI std::map<Texture *, Batch *> mBatchesStaticScreenSpace;

	void renderBatchesMap(std::map<Texture *, Batch *>& batchesMap);

PUB
	~BatchesMap() override;

	void init();
	void render();
	void addRenderer(Renderer * renderer);

	void setIsWorldSpace(bool isWorldSpace);
	void setIsStatic(bool isStatic);
	void forceRegenerateBuffers();
};