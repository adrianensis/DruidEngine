#pragma once

#include "Core/ObjectBase.hpp"

#include <map>

class Texture;
class Renderer;
class Batch;
template<class K, class V> class HashMap;

CLASS(BatchesMap, ObjectBase) {

	PRI(Batches, NONE, std::map<Texture*, Batch*>);

public:

	virtual ~BatchesMap() override;
	
	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};