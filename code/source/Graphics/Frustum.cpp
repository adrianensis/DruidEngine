#include "Frustum.h"

#include "Matrix4.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Array.h"

namespace DE {

// ---------------------------------------------------------------------------

Frustum::Frustum() : DE_Class() {

}

Frustum::~Frustum() {
	Memory::free<Array<Vector4>>(mPlanes);
	Memory::free<Matrix4>(mVPmatrix);
	mVPmatrix = nullptr;
	mCamera = nullptr;
}

void Frustum::init(Camera* camera){
	TRACE();

	mCamera = camera;
	mPlanes = Memory::allocate<Array<Vector4>>();
	mPlanes->init(6);

	for (u32 i = 0; i < mPlanes->getLength(); i++) {
			mPlanes->set(i, Vector4(0,0,0,0)); // x,y,z,w <=> A,B,C,D
	}

	mVPmatrix = Memory::allocate<Matrix4>();
	mVPmatrix->identity();
};

//----------------------------------------------------------------------

bool Frustum::testSphere(Vector3 center, f32 radius) const {

	for (u32 i = 0; i < mPlanes->getLength(); i++) {

		f32 A = mPlanes->get(i).x;
		f32 B = mPlanes->get(i).y;
		f32 C = mPlanes->get(i).z;
		f32 D = mPlanes->get(i).w;

		if((A*center.x)+(B*center.y)+(C*center.z)+D <= -radius)
			return false;
	}

	return true;
};

//----------------------------------------------------------------------

void Frustum::build(){

	Transform* t = mCamera->getGameObject()->getTransform();

	if(t->isDirtyTranslation()){

	  u32 LEFT = 0;
	  u32 RIGHT = 1;
	  u32 BOTTOM = 2;
	  u32 TOP = 3;
	  u32 NEAR = 4;
	  u32 FAR = 5;

	  mVPmatrix->init(mCamera->getViewTranslationMatrix());
	  mVPmatrix->mul(mCamera->getProjectionMatrix()/*.transpose()*/);

		/*
		* LEFT
		*/
	  mPlanes->set(LEFT, Vector4(
			mVPmatrix->get(0,0) + mVPmatrix->get(3,0),
		  mVPmatrix->get(0,1) + mVPmatrix->get(3,1),
		  mVPmatrix->get(0,2) + mVPmatrix->get(3,2),
			mVPmatrix->get(0,3) + mVPmatrix->get(3,3)
		));

		/*
		* RIGHT
		*/
	  mPlanes->set(RIGHT, Vector4(
			-mVPmatrix->get(0,0) + mVPmatrix->get(3,0),
		  -mVPmatrix->get(0,1) + mVPmatrix->get(3,1),
		  -mVPmatrix->get(0,2) + mVPmatrix->get(3,2),
			-mVPmatrix->get(0,3) + mVPmatrix->get(3,3)
		));

		/*
		* BOTTOM
		*/
	  mPlanes->set(BOTTOM, Vector4(
			mVPmatrix->get(1,0) + mVPmatrix->get(3,0),
		  mVPmatrix->get(1,1) + mVPmatrix->get(3,1),
		  mVPmatrix->get(1,2) + mVPmatrix->get(3,2),
		  mVPmatrix->get(1,3) + mVPmatrix->get(3,3)
		));

		/*
		* TOP
		*/
	  mPlanes->set(TOP, Vector4(
			-mVPmatrix->get(1,0) + mVPmatrix->get(3,0),
		  -mVPmatrix->get(1,1) + mVPmatrix->get(3,1),
		  -mVPmatrix->get(1,2) + mVPmatrix->get(3,2),
		  -mVPmatrix->get(1,3) + mVPmatrix->get(3,3)
		));

		/*
		* NEAR
		*/
	  mPlanes->set(NEAR, Vector4(
			mVPmatrix->get(2,0) + mVPmatrix->get(3,0),
		  mVPmatrix->get(2,1) + mVPmatrix->get(3,1),
		  mVPmatrix->get(2,2) + mVPmatrix->get(3,2),
		  mVPmatrix->get(2,3) + mVPmatrix->get(3,3)
		));

		/*
		* FAR
		*/
	  mPlanes->set(FAR, Vector4(
			-mVPmatrix->get(2,0) + mVPmatrix->get(3,0),
		  -mVPmatrix->get(2,1) + mVPmatrix->get(3,1),
		  -mVPmatrix->get(2,2) + mVPmatrix->get(3,2),
		  -mVPmatrix->get(2,3) + mVPmatrix->get(3,3)
		));


    for (u32 i = 0; i < mPlanes->getLength(); i++) {

			Vector4 v4(mPlanes->get(i));
			Vector3 v3(v4.x, v4.y, v4.z);

			if(v3.len() > 0) {

				//v4.w = v4.w/v3.len();

				//v3.nor();
				v4.set(v3.x, v3.y, v3.z ,v4.w);

				mPlanes->set(i, v4);
			}
    }
	}
};

// ---------------------------------------------------------------------------

} /* namespace DE */
