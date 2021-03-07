#pragma once

#include "Core/DE_Class.hpp"

namespace DE {

class Texture;
class Renderer;
class Batch;
template<class K, class V> class HashMap;

class BatchesMap: public DE_Class {
private:

	using TextureBatchMap = HashMap<Texture*, Batch*>;
	DE_M(Batches, TextureBatchMap*);

public:

	DE_CLASS_BODY(BatchesMap)

	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};
}

