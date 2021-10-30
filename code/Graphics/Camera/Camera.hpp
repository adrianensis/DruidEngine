#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"

class Frustum;

CLASS(Camera, Component)
{
	PRI_M(Matrix4, ProjectionMatrix, NONE)
	PRI_M(Matrix4, ViewTranslationMatrix, NONE)
	PRI_M(Matrix4, InversePVMatrix, NONE) // used in screen to world calculations.

	PRI_M(f32, Left, NONE)
	PRI_M(f32, Right, NONE)
	PRI_M(f32, Bottom, NONE)
	PRI_M(f32, Top, NONE)
	PRI_M(f32, Near, NONE)
	PRI_M(f32, Far, NONE)

	PRI_M(f32, Aspect, NONE)
	PRI_M(f32, Fov, NONE)
	
	PRI_M(bool, IsOrtho, NONE)

	PRI_M(Frustum *, Frustum, GET)
	PRI_M(f32, Zoom, GET)

PUB
	Camera();
	~Camera() override;

	void init() override;
	void onComponentAdded() override;
	void recalculate();

	const Matrix4 &getProjectionMatrix() const;
	const Matrix4 &getViewTranslationMatrix();
	const Matrix4 &getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	void onResize();

	Vector3 screenToWorld(const Vector2& screenPosition);
	void calculateInverseMatrix(bool forceCalculate = false);

	void setZoom(f32 zoom);
	void zoomIn(f32 zoomDelta);
	void zoomOut(f32 zoomDelta);
	void resetZoom()
	{
		mZoom = 1;
		setZoom(mZoom);
	};
};