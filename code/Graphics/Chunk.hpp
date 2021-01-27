#pragma once

#include "Core/DE_Class.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Texture;
class Renderer;
class Batch;
class Chunk;
template<class K, class V> class HashMap;
template<class T> class List;

class BatchesMap: public DE_Class {
private:

	using TextureBatchMap = HashMap<Texture*, Batch*>;
	DE_M(Batches, TextureBatchMap*);

public:

	DE_CLASS(BatchesMap)

	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};

class Chunk: public DE_Class {
private:

	DE_M(Renderers, List<Renderer*>*);
	DE_M(LeftTop, Vector3);
	DE_M(Size, f32);
	DE_M_GET(IsLoaded, bool)
	DE_M_GET(Center, Vector3)
	DE_M_GET(Radius, f32)

public:

	DE_CLASS(Chunk)

	void init();
	void set(const Vector3 &leftTop, f32 size);
	void load();
	void update(BatchesMap *batchesMap);
	void unload();
	bool containsRenderer(const Renderer *renderer, f32 epsilon = 0.0f) const;
	bool containsRendererSphere(const Renderer *renderer) const;
	void addRenderer(Renderer *renderer);

};

}

