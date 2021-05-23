#pragma once

#include "Core/ObjectBase.hpp"

#include <map>

class Texture;
class Renderer;
class Batch;
template<class K, class V> class HashMap;

CLASS(BatchesMap, ObjectBase) {

private:

	std::map<Texture*, Batch*> mBatches;

public:

	virtual ~BatchesMap() override;
	
	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};