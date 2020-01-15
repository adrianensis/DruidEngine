#include "Camera.h"

#include "Matrix4.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Transform.h"
#include "Frustum.h"

namespace DE {

// ---------------------------------------------------------------------------

Camera::Camera() : Component() {

}

Camera::~Camera() {
	Memory::free<Matrix4>(mViewTranslationMatrix);
	Memory::free<Frustum>(mFrustum);
}

void Camera::init(){
	TRACE();

	mProjectionMatrix = nullptr;

	mViewTranslationMatrix = Memory::allocate<Matrix4>();
	mViewTranslationMatrix->identity();

	mInversePVMatrix = Memory::allocate<Matrix4>();
	mInversePVMatrix->identity();

	mFrustum = Memory::allocate<Frustum>();
	mFrustum->init(this);
};

// ---------------------------------------------------------------------------

/**
* Return the frustum.
* @returns {Frustum} The frustum.
*/
// const Frustum* Camera::getFrustum(){
// 	return mFrustum;
// };

// ---------------------------------------------------------------------------

void Camera::setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far){
	Memory::free<Matrix4>(mProjectionMatrix);
	mProjectionMatrix = Memory::allocate<Matrix4>();
	mProjectionMatrix->ortho(left, right, bottom, top, near, far);

	mFrustum->build();
};

// ---------------------------------------------------------------------------

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov){
	Memory::free<Matrix4>(mProjectionMatrix);
	mProjectionMatrix = Memory::allocate<Matrix4>();
	mProjectionMatrix->perspective(near, far, aspect, fov);

	mFrustum->build();
};

// ---------------------------------------------------------------------------

const Matrix4& Camera::getProjectionMatrix() const{
  return *mProjectionMatrix;
};

// ---------------------------------------------------------------------------

const Matrix4& Camera::getViewTranslationMatrix(){

	if(getGameObject()->getTransform()->isDirtyTranslation()){
		Vector3 position = getGameObject()->getTransform()->getLocalPosition();
		mViewTranslationMatrix->translation(position * -1);
	}

	return *mViewTranslationMatrix;
};

const Matrix4& Camera::getViewRotationMatrix(){
	return getGameObject()->getTransform()->getRotationMatrix();
};

// ---------------------------------------------------------------------------

Vector3 Camera::screenToWorld(Vector2 screenPosition){

	Transform* transform = getGameObject()->getTransform();

	if(transform->isDirtyTranslation()){
		Matrix4 inverseProjectionMatrix;
		Matrix4 viewTranslationMatrix;
		Matrix4 viewRotationMatrix;

		inverseProjectionMatrix.init(getProjectionMatrix());
		viewTranslationMatrix.init(getViewTranslationMatrix());
		viewRotationMatrix.init(getViewRotationMatrix());

		viewTranslationMatrix.mul(viewRotationMatrix);
		inverseProjectionMatrix.mul(viewTranslationMatrix);

		inverseProjectionMatrix.invert();

		mInversePVMatrix->init(inverseProjectionMatrix);

    transform->setDirtyTranslation(false);
	}

	Vector4 v = mInversePVMatrix->mulVector(Vector4(screenPosition.x, screenPosition.y, 0, 1.0));

	v.x = v.x / v.w;
	v.y = v.y / v.w;
	v.z = v.z / v.w;

	return Vector3(v.x, v.y, v.z);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
