#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"

class Frustum;

CLASS(Camera, Component)
{
	PRI(ProjectionMatrix, NONE, Matrix4)
	PRI(ViewTranslationMatrix, NONE, Matrix4)
	PRI(InversePVMatrix, NONE, Matrix4) // used in screen to world calculations.

	PRI(Left, NONE, f32)
	PRI(Right, NONE, f32)
	PRI(Bottom, NONE, f32)
	PRI(Top, NONE, f32)
	PRI(Near, NONE, f32)
	PRI(Far, NONE, f32)

	PRI(Fov, NONE, f32)
	PRI(IsOrtho, NONE, bool)

	PRI(Frustum, GET, Frustum *)
	PRI(Zoom, GET, f32)

public:
	Camera();
	~Camera() override;

	void init() override;

	const Matrix4 &getProjectionMatrix() const;
	const Matrix4 &getViewTranslationMatrix();
	const Matrix4 &getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	Vector3 screenToWorld(Vector2 screenPosition);
	void calculateInverseMatrix(bool forceCalculate = false);

	void setZoom(f32 zoom);
	void resetZoom()
	{
		mZoom = 1;
		setZoom(mZoom);
	};
};