#pragma once

#include "Scenes/Component.hpp"

#include "Scenes/Transform.hpp"
#include "Graphics/Camera/Frustum.hpp"

class Camera: public Component
{
    GENERATE_METADATA(Camera)

	PRI Matrix4 mProjectionMatrix;
	PRI mutable Matrix4 mViewMatrix;
	PRI mutable Matrix4 mProjectionViewMatrix;
	PRI Matrix4 mInversePVMatrix; // used in screen to world calculations.

	PRI mutable bool mViewMatrixNeedsUpdate = true;
	PRI mutable bool mProjectionViewMatrixNeedsUpdate = true;
	PRI mutable bool mInversePVMatrixNeedsUpdate = true;

	PRI f32 mLeft = 0.0f;
	PRI f32 mRight = 0.0f;
	PRI f32 mBottom = 0.0f;
	PRI f32 mTop = 0.0f;
	PRI f32 mNear = 0.0f;
	PRI f32 mFar = 0.0f;

	PRI f32 mAspect = 0.0f;
	PRI f32 mFov = 0.0f;
	
	PRI bool mIsOrtho = false;

	PRI Frustum mFrustum; GETREF_CONST(Frustum)
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
	const Matrix4 &getViewMatrix() const;
	const Matrix4 &getProjectionViewMatrix() const;

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