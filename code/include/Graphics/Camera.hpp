#ifndef DE_CAMERA_H
#define DE_CAMERA_H

#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Matrix4.hpp"
#include "Memory.hpp"

namespace DE {

class Matrix4;
class Frustum;

class Camera : public Component{

private:

	Matrix4 mProjectionMatrix;
	Matrix4 mViewTranslationMatrix;
	Matrix4 mInversePVMatrix; // used in screen to world calculations.
	Frustum* mFrustum;

	f32 mLeft;
	f32 mRight;
	f32 mBottom;
	f32 mTop;
	f32 mNear;
	f32 mFar;

	f32 mFov;

	bool mIsOrtho;
	f32 mZoom;

public:

	DE_CLASS(Camera, Component);

	void init() override;

	const Matrix4& getProjectionMatrix() const;
	const Matrix4& getViewTranslationMatrix();
	const Matrix4& getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	Vector3 screenToWorld(Vector2 screenPosition);
	void calculateInverseMatrix(bool forceCalculate = false);

	Frustum* getFrustum() const { return mFrustum; };

	void setZoom(f32 zoom);
	void resetZoom(){ mZoom = 1; setZoom(mZoom); };
	f32 getZoom(){ return mZoom; };
};

} /* namespace DE */

#endif /* DE_CAMERA_H */
