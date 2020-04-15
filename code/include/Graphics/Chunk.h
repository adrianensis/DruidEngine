#ifndef DE_CHUNK_H_
#define DE_CHUNK_H_

#include "DE_Class.h"
#include "Vector3.h"

namespace DE {

class Texture;
class Renderer;
class Batch;
class Chunk;
template <class K, class V> class HashMap;
template <class T> class List;

class BatchesMap : public DE_Class {
	private:
		HashMap<Texture*, Batch*>* mBatches;

	public:
		DE_CLASS(BatchesMap, DE_Class);

		void init();
		u32 render(u32 layer);
		void addRenderer(Renderer* renderer);
};

class Chunk : public DE_Class {
	private:
		List<Renderer*>* mRenderers;
		// BatchesMap* mBatchesMap;
		u32 mLastRenderersSize;
		u32 mNewRenderersSize;

  public:
    DE_CLASS(Chunk, DE_Class);

		Vector3 mLeftTop;
		Vector3 mCenter;
		f32 mRadius;
		f32 mSize;

		bool mIsLoaded;

    void init();
		void set(const Vector3& leftTop, f32 size);
    void load();
    void update(BatchesMap* batchesMap);
    void unload();
    bool isLoaded();
		bool containsRenderer(const Renderer* renderer, f32 epsilon = 0.0f);
		bool containsRendererSphere(const Renderer* renderer);
		void addRenderer(Renderer* renderer);
		u32 render(u32 layer);
};

} /* namespace DE */

#endif /* DE_CHUNK_H_ */
