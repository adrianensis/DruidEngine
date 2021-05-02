#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Renderer;
class BatchesMap;
template<class T> class List;

class Chunk : public ObjectBase {
private:

	 List<Renderer*>* mRenderers;
	 Vector3 mLeftTop;
	 f32 mSize;
	 bool mIsLoaded;
	 Vector3 mCenter;
	 f32 mRadius;

public:

	GENERATE_METADATA(Chunk);

	Chunk();
	virtual ~Chunk() override;;

	GET(IsLoaded);
	GET(Center);
	GET(Radius);

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