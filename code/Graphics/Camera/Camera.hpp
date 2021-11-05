#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"
#include "Scene/Transform.hpp"

class Frustum;

class Camera: public Component
{
    GENERATE_METADATA(Camera)

	PRI Matrix4 mProjectionMatrix;
	PRI Matrix4 mViewTranslationMatrix;
	PRI Matrix4 mInversePVMatrix; // used in screen to world calculations.
	PRI bool mInversePVMatrixNeedsUpdate = {true};

	PRI f32 mLeft = 0.0f;
	PRI f32 mRight = 0.0f;
	PRI f32 mBottom = 0.0f;
	PRI f32 mTop = 0.0f;
	PRI f32 mNear = 0.0f;
	PRI f32 mFar = 0.0f;

	PRI f32 mAspect = 0.0f;
	PRI f32 mFov = 0.0f;
	
	PRI bool mIsOrtho = false;

	PRI Frustum* mFrustum = nullptr; GET(Frustum)
	PRI f32 mZoom = 0.0f; GET(Zoom)

	PRI TransformState mTransformState;

	PRI void calculateInverseMatrix(bool force = false);

PUB
	Camera();
	~Camera() override;

	void init() override;
	void onComponentAdded() override;
	void recalculatePerspectiveMatrix();

	void update();

	const Matrix4 &getProjectionMatrix() const;
	const Matrix4 &getViewTranslationMatrix();
	const Matrix4 &getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	void onResize();

	Vector3 screenToWorld(const Vector2& screenPosition);

	void setZoom(f32 zoom);
	void zoomIn(f32 zoomDelta);
	void zoomOut(f32 zoomDelta);
	void resetZoom()
	{
		mZoom = 1;
		setZoom(mZoom);
	};
};