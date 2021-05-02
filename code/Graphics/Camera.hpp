#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"
#include "Memory/Memory.hpp"

namespace DE {

class Matrix4;
class Frustum;

class Camera : public Component {

private:

	 Matrix4 mProjectionMatrix;
	 Matrix4 mViewTranslationMatrix;
	 Matrix4 mInversePVMatrix; // used in screen to world calculations.

	 f32 mLeft;
	 f32 mRight;
	 f32 mBottom;
	 f32 mTop;
	 f32 mNear;
	 f32 mFar;
	
	 f32 mFov;
	 bool mIsOrtho;

	 Frustum* mFrustum;
	 f32 mZoom;

public:

	GENERATE_METADATA(Camera);

	Camera();
	virtual ~Camera() override;;

	GET(Frustum);
	GET(Zoom);

	void init() override;

	const Matrix4& getProjectionMatrix() const;
	const Matrix4& getViewTranslationMatrix();
	const Matrix4& getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	Vector3 screenToWorld(Vector2 screenPosition);
	void calculateInverseMatrix(bool forceCalculate = false);

	void setZoom(f32 zoom);
	void resetZoom() {
		mZoom = 1;
		setZoom(mZoom);
	};
};
}

