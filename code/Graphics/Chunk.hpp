#ifndef DE_CHUNK_H_
#define DE_CHUNK_H_

#include "DE_Class.hpp"
#include "Vector3.hpp"

namespace DE {

class Texture;
class Renderer;
class Batch;
class Chunk;
template<class K, class V> class HashMap;
template<class T> class List;

class BatchesMap: public DE_Class {
private:

	HashMap<Texture*, Batch*>* mBatches;

public:

	DE_CLASS(BatchesMap, DE_Class)
	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};

class Chunk: public DE_Class {
private:

	List<Renderer*>* mRenderers;
	Vector3 mLeftTop;
	f32 mSize;
	bool mIsLoaded;
	Vector3 mCenter;
	f32 mRadius;

public:

	DE_CLASS(Chunk, DE_Class)

	DE_GET(IsLoaded)
	DE_GET(Center)
	DE_GET(Radius)

	void init();
	void set(const Vector3 &leftTop, f32 size);
	void load();
	void update(BatchesMap *batchesMap);
	void unload();
	bool containsRenderer(const Renderer *renderer, f32 epsilon = 0.0f) const;
	bool containsRendererSphere(const Renderer *renderer) const;
	void addRenderer(Renderer *renderer);

};

} /* namespace DE */

#endif /* DE_CHUNK_H_ */
