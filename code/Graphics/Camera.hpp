#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"
#include "Memory/Memory.hpp"

namespace DE {

class Matrix4;
class Frustum;

class Camera: public Component {

private:

	DE_M(ProjectionMatrix, Matrix4)
	DE_M(ViewTranslationMatrix, Matrix4)
	DE_M(InversePVMatrix, Matrix4) // used in screen to world calculations.

	DE_M(Left, f32)
	DE_M(Right, f32)
	DE_M(Bottom, f32)
	DE_M(Top, f32)
	DE_M(Near, f32)
	DE_M(Far, f32)
	
	DE_M(Fov, f32)
	DE_M(IsOrtho, bool)

	DE_M_GET(Frustum, Frustum*)
	DE_M_GET(Zoom, f32)

public:

	DE_CLASS(Camera)

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

