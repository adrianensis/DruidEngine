#pragma once

#include "Core/ObjectBase.hpp"

#include <map>

class Texture;
class Renderer;
class Batch;
template<class K, class V> class HashMap;

CLASS(BatchesMap, ObjectBase) {

	PRI(Batches, NONE, std::map<Texture*, Batch*>);
	PRI(IsWorldSpace, GET, bool)

public:

	virtual ~BatchesMap() override;
	
	void init();
	void render(u32 layer);
	void addRenderer(Renderer *renderer);

	void setIsWorldSpace(bool isWorldSpace);
};