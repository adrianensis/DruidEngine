#include "Camera.h"

#include "Matrix4.h"
#include "GameObject.h"
#include "Transform.h"

namespace DE {

Camera::Camera() : Component() {
	// TODO Auto-generated constructor stub

}

Camera::~Camera() {
	Memory::free<Matrix4>(mViewTranslationMatrix);
}

void Camera::init(){
	mViewTranslationMatrix = Memory::allocate<Matrix4>();
	mViewTranslationMatrix->identity();
};

//----------------------------------------------------------------------

/**
* Return the frustum.
* @returns {Frustum} The frustum.
*/
// const Frustum* Camera::getFrustum(){
// 	return mFrustum;
// };

//----------------------------------------------------------------------

void Camera::setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far){
	Memory::free<Matrix4>(mProjectionMatrix);
	mProjectionMatrix = Memory::allocate<Matrix4>();
	mProjectionMatrix->ortho(left, right, bottom, top, near, far);
};

//----------------------------------------------------------------------

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov){
	Memory::free<Matrix4>(mProjectionMatrix);
	mProjectionMatrix = Memory::allocate<Matrix4>();
	mProjectionMatrix->perspective(near, far, aspect, fov);
};

//----------------------------------------------------------------------

const Matrix4& Camera::getProjectionMatrix() const{
  return *mProjectionMatrix;
};

//----------------------------------------------------------------------

const Matrix4& Camera::getViewTranslationMatrix(){

	// TODO : optimization, if transform hasn't changed, directly return mViewTranslationMatrix

	Vector3 position = getGameObject()->getTransform()->getLocalPosition();
	mViewTranslationMatrix->translation(position * -1);

	return *mViewTranslationMatrix;
};

const Matrix4& Camera::getViewRotationMatrix(){
	return getGameObject()->getTransform()->getRotationMatrix();
};

//----------------------------------------------------------------------

} /* namespace DE */
