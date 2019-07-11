#include "Camera.h"

#include "Matrix4.h"

namespace DE {

Camera::Camera() : Component() {
	// TODO Auto-generated constructor stub

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::init(){

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
  mProjectionMatrix = Memory::allocate<Matrix4>();
  mProjectionMatrix->ortho(left, right, bottom, top, near, far);
};

//----------------------------------------------------------------------

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov){
  mProjectionMatrix = Memory::allocate<Matrix4>();
  mProjectionMatrix->perspective(near, far, aspect, fov);
};

//----------------------------------------------------------------------

void Camera::setProjection(Matrix4* projectionMatrix){
    mProjectionMatrix = projectionMatrix;
};

//----------------------------------------------------------------------

const Matrix4* Camera::getProjectionMatrix() const{
    return mProjectionMatrix;
};

//----------------------------------------------------------------------

const Matrix4* Camera::getViewMatrix(){

	// var t = this.gameObject.getTransform();
  //
	// if(t.isDirty()){
  //
	// 	var rotationMatrix = new Matrix4(
	//         t.right,
	//         t.up,
	//         t.forward,
	//         new Vector4(0,0,0,1));
  //
	// 	var translationMatrix = Matrix4.translation(t.getPosition().cpy().mulScl(-1));
  //
	// 	mViewMatrix = Matrix4.mulMM(translationMatrix, rotationMatrix);
	// }

	return mViewMatrix;
};

//----------------------------------------------------------------------

} /* namespace DE */
