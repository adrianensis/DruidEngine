#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"

class Frustum;

CLASS(Camera, Component)
{
	PRI(Matrix4, ProjectionMatrix, NONE)
	PRI(Matrix4, ViewTranslationMatrix, NONE)
	PRI(Matrix4, InversePVMatrix, NONE) // used in screen to world calculations.

	PRI(f32, Left, NONE)
	PRI(f32, Right, NONE)
	PRI(f32, Bottom, NONE)
	PRI(f32, Top, NONE)
	PRI(f32, Near, NONE)
	PRI(f32, Far, NONE)

	PRI(f32, Aspect, NONE)
	PRI(f32, Fov, NONE)
	
	PRI(bool, IsOrtho, NONE)

	PRI(Frustum *, Frustum, GET)
	PRI(f32, Zoom, GET)

public:
	Camera();
	~Camera() OVR;

	void init() OVR;
	void onComponentAdded() OVR;
	void recalculate();

	CNS Matrix4 &getProjectionMatrix() CNS;
	CNS Matrix4 &getViewTranslationMatrix();
	CNS Matrix4 &getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	void onResize();

	Vector3 screenToWorld(CNS Vector2& screenPosition);
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