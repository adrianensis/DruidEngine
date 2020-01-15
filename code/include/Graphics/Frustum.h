#ifndef DE_FRUSTUM_H
#define DE_FRUSTUM_H

#include "DE_Class.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Memory.h"

namespace DE {

class Matrix4;
class Camera;
template <class T> class Array;

class Frustum : public DE_Class{

private:

Matrix4* mVPmatrix;
Camera* mCamera;

Array<Vector4>* mPlanes;

public:

	DE_CLASS(Frustum, DE_Class);

	void init(Camera* camera);

	/**
	* Builds the frustum planes.
	*/
	void build();

	bool testSphere(Vector3 center, f32 radius) const;
};

} /* namespace DE */

#endif /* DE_FRUSTUM_H */
